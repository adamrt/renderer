#include "SDL_events.h"
#include "SDL_timer.h"
#include <algorithm>
#include <iostream>

#include "Engine.h"
#include "Light.h"
#include "Mesh.h"
#include "Triangle.h"
#include "Vector.h"

bool left_button_down = false;

Engine::Engine(Window& window, Framebuffer& fb, Camera& camera, UI& ui, Scene& scene)
    : m_window(window)
    , m_framebuffer(fb)
    , m_camera(camera)
    , m_ui(ui)
    , m_scene(scene)
{
    // Callback for when the projection type changes in the GUI.
    m_ui.orientation_event = [&]() {
        reset_orientation();
    };
}

void Engine::setup()
{

    i32 triangles = 0;
    for (auto& model : m_scene.models) {
        triangles += model.mesh.triangles.size();
    }
    m_ui.total_triangles = triangles;
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
            if (mx < m_framebuffer.scaled_width() && my < m_framebuffer.scaled_height()) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    left_button_down = true;
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (mx < m_framebuffer.scaled_width() && my < m_framebuffer.scaled_height()) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    left_button_down = false;
                }
            }
            break;
        case SDL_MOUSEMOTION:
            if (mx < m_framebuffer.scaled_width() && my < m_framebuffer.scaled_height()) {
                if (left_button_down) {
                    m_ui.rotate = false;
                    m_camera.orbit(event.motion.xrel * 0.25f, event.motion.yrel * 0.25f);
                    m_camera.update();
                }
            }
            break;
        case SDL_MOUSEWHEEL:
            if (mx < m_framebuffer.scaled_width() && my < m_framebuffer.scaled_height()) {
                m_camera.zoom(-event.wheel.preciseY);
                m_camera.update();
            }
            break;
        }
    }
}

void Engine::update()
{
    i32 delay = target_frame_time() - (SDL_GetTicks() - m_previous_frame_time);

    if (delay > 0 && delay <= target_frame_time()) {
        SDL_Delay(delay);
    }

    m_previous_frame_time = SDL_GetTicks();
    f32 half_w = m_framebuffer.width() / 2.0f;
    f32 half_h = m_framebuffer.height() / 2.0f;

    for (auto& model : m_scene.models) {

        model.triangles_to_render.clear();

        if (m_ui.rotate) {
            if (m_ui.rotate_x) {
                model.rotation.x += 0.008f;
            }
            if (m_ui.rotate_y) {
                model.rotation.y += 0.008f;
            }
            if (m_ui.rotate_z) {
                model.rotation.z += 0.008f;
            }
        }

        Mat4 world = Mat4::world(model.scale, model.rotation, model.translation);

        for (auto& tri : model.mesh.triangles) {

            auto trans_verts = std::array<Vec3, 3> {};
            for (i32 i = 0; i < 3; i++) {
                trans_verts[i] = world * tri.vertices[i].position.xyz();
            }

            Vec3 triangle_normal = vertices_normal(trans_verts);

            Color diffuse;
            Color light_sum(0.0f);
            for (auto& light : m_scene.lights) {
                Vec3 light_dir = (light.position - trans_verts[0]).normalized();
                f32 diffuse_intensity = std::max(Vec3::dot(triangle_normal, light_dir), 0.0f);
                Color diffuse = light.color * diffuse_intensity;
                light_sum = light_sum + diffuse;
            }

            for (i32 i = 0; i < 3; i++) {
                trans_verts[i] = m_camera.view * trans_verts[i];
            }

            Triangle triangle;

            for (i32 i = 0; i < 3; i++) {
                Vec4 proj_vertex = m_camera.projection_matrix() * trans_verts[i].xyzw();

                if (m_camera.projection == Projection::Perspective) {
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

                triangle.vertices[i].position = proj_vertex;
            }

            if (m_ui.backface_culling && should_cull(triangle.vertices)) {
                continue;
            }

            triangle.light_sum = light_sum;

            if (model.texture.is_valid()) {
                triangle.vertices[0].uv = tri.vertices[0].uv;
                triangle.vertices[1].uv = tri.vertices[1].uv;
                triangle.vertices[2].uv = tri.vertices[2].uv;
            }

            triangle.vertices[0].normal = tri.vertices[0].normal;
            triangle.vertices[1].normal = tri.vertices[1].normal;
            triangle.vertices[2].normal = tri.vertices[2].normal;

            model.triangles_to_render.push_back(triangle);
        }
    }

    m_ui.visible_triangles = 0; // triangles_to_render.size();
}

void Engine::render()
{
    m_framebuffer.clear_depthbuffer();
    m_framebuffer.clear_colorbuffer(Color::DarkGray);
    m_framebuffer.draw_grid(Color::LightGray);

    for (auto& model : m_scene.models) {
        for (auto& t : model.triangles_to_render) {

            if (m_ui.draw_texture) {
                if (model.texture.is_valid()) {
                    m_framebuffer.draw_triangle_textured(t, model.texture);
                } else {
                    Color color(m_ui.fill_color);
                    m_framebuffer.draw_triangle_filled(t, color);
                }

            } else if (m_ui.draw_filled) {
                Color color(m_ui.fill_color);
                m_framebuffer.draw_triangle_filled(t, color);
            } else if (m_ui.draw_normals) {
                m_framebuffer.draw_triangle_normals(t);
            }

            if (m_ui.draw_wireframe) {
                m_framebuffer.draw_triangle(
                    t.vertices[0].position.x, t.vertices[0].position.y,
                    t.vertices[1].position.x, t.vertices[1].position.y,
                    t.vertices[2].position.x, t.vertices[2].position.y,
                    Color::Purple);
            }
        }
    }

    m_window.update_texture();
    m_ui.render();
    m_window.render();
}

void Engine::reset_orientation()
{
    for (auto& model : m_scene.models) {
        model.rotation.x = 0.0f;
        model.rotation.y = 0.0f;
        model.rotation.z = 0.0f;
    }
}
