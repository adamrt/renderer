#include "SDL_events.h"
#include "SDL_timer.h"
#include <algorithm>
#include <iostream>

#include "Engine.h"
#include "Light.h"
#include "Mesh.h"
#include "Triangle.h"
#include "Vector.h"

std::vector<Triangle> triangles_to_render {};
Light light { Vec3(0.0f, 0.0f, 1.0f), Color::Blue };

Mesh mesh("res/cube.obj");
Texture texture("res/cube.png");

bool left_button_down = false;

Engine::Engine(Framebuffer& fb, Window& window, Camera& camera, UI& ui)
    : m_framebuffer(fb)
    , m_window(window)
    , m_camera(camera)
    , m_ui(ui)
{
    // Callback for when the projection type changes in the GUI.
    m_ui.update_camera = [&]() {
        m_camera.update();
    };

    m_ui.orientation_event = [&]() {
        reset_orientation();
    };
}

void Engine::setup()
{
    m_ui.total_triangles = mesh.faces.size();
    m_camera.update();
}

void Engine::process_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        i32 mx, my;
        SDL_GetMouseState(&mx, &my);

        switch (event.type) {
        case SDL_QUIT:
            m_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                m_running = false;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (mx < m_framebuffer.width() && my < m_framebuffer.height()) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    left_button_down = true;
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (mx < m_framebuffer.width() && my < m_framebuffer.height()) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    left_button_down = false;
                }
            }
            break;
        case SDL_MOUSEMOTION:
            if (mx < m_framebuffer.width() && my < m_framebuffer.height()) {
                if (left_button_down) {
                    m_ui.rotate = false;
                    m_camera.orbit(event.motion.xrel * 0.25f, event.motion.yrel * 0.25f);
                    m_camera.update();
                }
            }
            break;
        case SDL_MOUSEWHEEL:
            if (mx < m_framebuffer.width() && my < m_framebuffer.height()) {
                m_camera.zoom(-event.wheel.preciseY);
                m_camera.update();
            }
            break;
        }
    }
}

void Engine::update()
{
    triangles_to_render.clear();

    i32 delay = FRAME_TARGET_TIME - (SDL_GetTicks() - m_previous_frame_time);

    if (delay > 0 && delay <= FRAME_TARGET_TIME) {
        SDL_Delay(delay);
    }

    m_previous_frame_time = SDL_GetTicks();

    if (m_ui.rotate) {
        if (m_ui.rotate_x) {
            mesh.rotation.x += 0.008f;
        }
        if (m_ui.rotate_y) {
            mesh.rotation.y += 0.008f;
        }
        if (m_ui.rotate_z) {
            mesh.rotation.z += 0.008f;
        }
    }

    f32 half_w = m_framebuffer.width() / 2.0f;
    f32 half_h = m_framebuffer.height() / 2.0f;

    Mat4 world = Mat4::world(mesh.scale, mesh.rotation, mesh.translation);

    for (auto& face : mesh.faces) {
        auto face_verts = std::array<Vec3, 3> {
            mesh.vertices[face.a - 1],
            mesh.vertices[face.b - 1],
            mesh.vertices[face.c - 1]
        };

        auto trans_verts = std::array<Vec3, 3> {};
        for (i32 i = 0; i < 3; i++) {
            trans_verts[i] = world * face_verts[i];
        }

        Vec3 normal = vertices_normal(trans_verts);

        for (i32 i = 0; i < 3; i++) {
            trans_verts[i] = m_camera.view_matrix * trans_verts[i];
        }

        auto proj_verts = std::array<Vec4, 3> {};

        for (i32 i = 0; i < 3; i++) {
            Vec4 proj_vertex = m_camera.proj_matrix * trans_verts[i].xyzw();

            if (m_ui.projection == Projection::Perspective) {
                if (proj_vertex.w != 0.0f) {
                    proj_vertex.x /= proj_vertex.w;
                    proj_vertex.y /= proj_vertex.w;
                    proj_vertex.z /= proj_vertex.w;
                }
            }

            // Invert the Y asis to compensate for the Y axis of the model and
            // the color buffer being different (+Y up vs +Y down, respectively).
            proj_vertex.y *= -1;

            // Scale to screen size
            proj_vertex.x *= half_w;
            proj_vertex.y *= half_h;

            // Translate to middle of the screen
            proj_vertex.x += half_w;
            proj_vertex.y += half_h;

            proj_verts[i] = proj_vertex;
        }

        if (m_ui.backface_culling && should_cull(proj_verts)) {
            continue;
        }

        Triangle triangle;
        triangle.points = proj_verts;

        if (texture.is_valid()) {
            triangle.texcoords[0] = mesh.texcoords[face.ta - 1];
            triangle.texcoords[1] = mesh.texcoords[face.tb - 1];
            triangle.texcoords[2] = mesh.texcoords[face.tc - 1];
        }

        // Light
        triangle.light_intensity = -Vec3::dot(normal, light.direction);

        triangles_to_render.push_back(triangle);
    }

    m_ui.visible_triangles = triangles_to_render.size();
}

void Engine::render()
{
    m_framebuffer.clear_depthbuffer();
    m_framebuffer.clear_colorbuffer(Color::DarkGray);
    m_framebuffer.draw_grid(Color::LightGray);

    for (auto& t : triangles_to_render) {

        if (m_ui.draw_texture) {
            if (texture.is_valid()) {
                m_framebuffer.draw_triangle_textured(t, texture);
            } else {
                Color color(m_ui.fill_color);
                m_framebuffer.draw_triangle_filled(t, color);
            }

        } else if (m_ui.draw_filled) {
            Color color(m_ui.fill_color);
            m_framebuffer.draw_triangle_filled(t, color);
        }

        if (m_ui.draw_wireframe) {
            m_framebuffer.draw_triangle(
                t.points[0].x, t.points[0].y,
                t.points[1].x, t.points[1].y,
                t.points[2].x, t.points[2].y,
                Color::Purple);
        }
    }

    m_window.render();
}

void Engine::reset_orientation()
{
    mesh.rotation.x = 0.0f;
    mesh.rotation.y = 0.0f;
    mesh.rotation.z = 0.0f;
}
