#include "SDL_events.h"
#include "SDL_timer.h"
#include <algorithm>
#include <iostream>

#include "Engine.h"
#include "Light.h"
#include "Matrix.h"
#include "Mesh.h"
#include "Triangle.h"
#include "UI.h"
#include "Vector.h"
#include "Window.h"

const float ZNEAR = 0.1f;
const float ZFAR = 100.0f;

const float MIN_ZOOM = 0.5f;
const float MAX_ZOOM = 2.5f;

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

    // Initial projection matrix
    update_projection();
}

void Engine::setup()
{
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
            float y = event.wheel.preciseY;
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

    int delay = FRAME_TARGET_TIME - (SDL_GetTicks() - m_previous_frame_time);

    if (delay > 0 && delay <= FRAME_TARGET_TIME) {
        SDL_Delay(delay);
    }

    m_previous_frame_time = SDL_GetTicks();

    if (m_ui.rotate) {
        mesh.rotation.x += 0.008f;
        mesh.rotation.y += 0.008f;
        mesh.rotation.z += 0.008f;
    }

    // Push points away from the camera.
    mesh.translation.z = 5.0f;

    float half_w = m_framebuffer.width() / 2.0f;
    float half_h = m_framebuffer.height() / 2.0f;

    Mat4 world = Mat4::world(mesh.scale, mesh.rotation, mesh.translation);

    for (auto& face : mesh.faces) {
        auto vertices = std::array<Vec3, 3> {
            mesh.vertices[face.a - 1],
            mesh.vertices[face.b - 1],
            mesh.vertices[face.c - 1]
        };

        // Transform
        for (int i = 0; i < 3; i++) {
            vertices[i] = world * vertices[i];
        }

        Triangle proj_triangle;
        proj_triangle.color = face.color;
        proj_triangle.avg_depth = (vertices[0].z + vertices[1].z + vertices[2].z) / 3.0f;
        proj_triangle.texcoords[0] = mesh.texcoords[face.ta - 1];
        proj_triangle.texcoords[1] = mesh.texcoords[face.tb - 1];
        proj_triangle.texcoords[2] = mesh.texcoords[face.tc - 1];

        // Project
        for (int i = 0; i < 3; i++) {
            Vec4 proj_vertex = m_projection_matrix * vertices[i].xyzw();

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

            proj_triangle.points[i] = proj_vertex.xy();
        }

        if (m_ui.backface_culling && proj_triangle.should_cull()) {
            continue;
        }

        Vec3 normal = vertices_normal(vertices);
        float intensity = -Vec3::dot(normal, light.direction);
        proj_triangle.color = face.color * intensity;

        triangles_to_render.push_back(proj_triangle);
    }

    // Painters algo
    std::sort(triangles_to_render.begin(), triangles_to_render.end(), [](const Triangle& a, const Triangle& b) {
        return a.avg_depth > b.avg_depth;
    });
}

void Engine::render()
{
    m_framebuffer.clear(Color::DarkGray);
    m_framebuffer.draw_grid(Color::LightGray);

    if (m_ui.draw_filled) {
        for (auto& t : triangles_to_render) {
            m_framebuffer.draw_triangle_textured(t, texture);
        }
    }

    if (m_ui.draw_wireframe) {
        for (auto& t : triangles_to_render) {
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
        float fov = M_PI / 3.0 * zoom;
        m_projection_matrix = Mat4::perspective(fov, m_framebuffer.aspect(), ZNEAR, ZFAR);
    } else {
        float w = 1.0f * zoom;
        float h = 1.0f * m_framebuffer.aspect() * zoom;
        m_projection_matrix = Mat4::orthographic(-w, w, -h, h, ZNEAR, ZFAR);
    }
};
