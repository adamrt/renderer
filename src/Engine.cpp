#include "SDL_events.h"
#include "SDL_timer.h"
#include <algorithm>
#include <iostream>

#include "Engine.h"
#include "Light.h"
#include "Mesh.h"
#include "Triangle.h"
#include "Vector.h"

const f32 ZNEAR = 0.1f;
const f32 ZFAR = 100.0f;

const f32 MIN_ZOOM = 0.1f;
const f32 MAX_ZOOM = 2.5f;

std::vector<Triangle> triangles_to_render {};
Light light { Vec3(0.0f, 0.0f, 1.0f), Color::Blue };

Mesh mesh("res/cube.obj");
Texture texture("res/cube.png");

Engine::Engine(Framebuffer& fb, Window& window, UI& ui)
    : m_framebuffer(fb)
    , m_window(window)
    , m_ui(ui)
{
    // Callback for when the projection type changes in the GUI.
    m_window.proj_event = [&]() {
        update_projection();
    };

    m_window.orientation_event = [&]() {
        reset_orientation();
    };

    // Initial projection matrix
    update_projection();
}

void Engine::setup()
{
    m_ui.total_triangles = mesh.faces.size();
}

void Engine::process_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        switch (event.type) {
        case SDL_QUIT:
            m_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                m_running = false;
            }
            break;
        case SDL_MOUSEWHEEL:
            f32 y = event.wheel.preciseY;
            y *= 0.1f;
            zoom -= y;
            if (zoom < MIN_ZOOM)
                zoom = MIN_ZOOM;
            if (zoom > MAX_ZOOM)
                zoom = MAX_ZOOM;

            update_projection();
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
    Mat4 view = Mat4::look_at(m_ui.camera_position, Vec3(), Vec3(0.0f, 1.0f, 0.0));

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
            trans_verts[i] = view * trans_verts[i];
        }

        auto proj_verts = std::array<Vec4, 3> {};

        for (i32 i = 0; i < 3; i++) {
            Vec4 proj_vertex = m_projection_matrix * trans_verts[i].xyzw();

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
        triangle.texcoords[0] = mesh.texcoords[face.ta - 1];
        triangle.texcoords[1] = mesh.texcoords[face.tb - 1];
        triangle.texcoords[2] = mesh.texcoords[face.tc - 1];

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
            m_framebuffer.draw_triangle_textured(t, texture);

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

void Engine::update_projection()
{
    if (m_ui.projection == Projection::Perspective) {
        f32 fov = M_PI / 3.0 * zoom;
        m_projection_matrix = Mat4::perspective(fov, m_framebuffer.aspect(), ZNEAR, ZFAR);
    } else {
        f32 w = 1.0f * zoom;
        f32 h = 1.0f * m_framebuffer.aspect() * zoom;
        m_projection_matrix = Mat4::orthographic(-w, w, -h, h, ZNEAR, ZFAR);
    }
};

void Engine::reset_orientation()
{
    mesh.rotation.x = 0.0f;
    mesh.rotation.y = 0.0f;
    mesh.rotation.z = 0.0f;
}
