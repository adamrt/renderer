#include "SDL_timer.h"
#include <iostream>

#include "Engine.h"
#include "Matrix.h"
#include "Mesh.h"
#include "Triangle.h"
#include "Vector.h"
#include "Window.h"

float fov_factor = 640.0f;

std::vector<Triangle> triangles_to_render {};

Mesh mesh(
    {
        { -1, -1, -1 }, // 1
        { -1, 1, -1 },  // 2
        { 1, 1, -1 },   // 3
        { 1, -1, -1 },  // 4
        { 1, 1, 1 },    // 5
        { 1, -1, 1 },   // 6
        { -1, 1, 1 },   // 7
        { -1, -1, 1 }   // 8
    },
    {
        { 1, 2, 3, 0xFF0000FF }, // front
        { 1, 3, 4, 0x00FF00FF },
        { 4, 3, 5, 0x0000FFFF }, // right
        { 4, 5, 6, 0xFFFF00FF },
        { 6, 5, 7, 0x00FFFFFF }, // back
        { 6, 7, 8, 0xA500FFFF },
        { 8, 7, 2, 0xFFFFFFFF }, // left
        { 8, 2, 1, 0x990000FF },
        { 2, 7, 5, 0x009900FF }, // top
        { 2, 5, 3, 0x000099FF },
        { 6, 8, 1, 0x999900FF }, // bottom
        { 6, 1, 4, 0x009999FF },
    });

Engine::Engine(Framebuffer& fb, Window& window, UI& ui)
    : m_framebuffer(fb)
    , m_window(window)
    , m_ui(ui)
{
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
                break;
            }
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

    auto project = [](Vec3 p) {
        return Vec2(p.x / p.z * fov_factor, p.y / p.z * fov_factor);
    };

    if (m_ui.rotate) {
        mesh.rotation.x += 0.008f;
        mesh.rotation.y += 0.008f;
        mesh.rotation.z += 0.008f;
    }

    auto w = m_framebuffer.width();
    auto h = m_framebuffer.height();

    auto world = Mat4::world(mesh.scale, mesh.rotation, mesh.translation);

    for (auto& face : mesh.faces) {
        auto face_vertices = std::vector<Vec3> {
            mesh.vertices[face.a - 1],
            mesh.vertices[face.b - 1],
            mesh.vertices[face.c - 1]
        };

        Triangle projected_triangle {};
        projected_triangle.color = face.color;

        for (auto transformed : face_vertices) {
            transformed = world * transformed;

            // Push points away from the camera.
            transformed.z += 5.0f;

            Vec2 projected = project(transformed);

            // Invert the Y asis to compensate for the Y axis of the model and
            // the color buffer being different (+Y up vs +Y down, respectively).
            projected.y *= -1;

            projected.x += (w / 2.0f);
            projected.y += (h / 2.0f);

            projected_triangle.points.push_back(projected);
        }

        if (m_ui.backface_culling && projected_triangle.should_cull()) {
            continue;
        }

        triangles_to_render.push_back(projected_triangle);
    }
}

void Engine::render()
{
    m_framebuffer.clear(0x333333FF);

    if (m_ui.draw_filled) {
        for (auto& t : triangles_to_render) {
            m_framebuffer.draw_triangle_filled(
                t.points[0].x, t.points[0].y,
                t.points[1].x, t.points[1].y,
                t.points[2].x, t.points[2].y,
                t.color);
        }
    } else {
        for (auto& t : triangles_to_render) {
            m_framebuffer.draw_triangle(
                t.points[0].x, t.points[0].y,
                t.points[1].x, t.points[1].y,
                t.points[2].x, t.points[2].y,
                0xA500FFFF);
        }
    }

    m_window.render();
}
