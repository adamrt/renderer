#include "Engine.h"
#include "SDL_timer.h"
#include "Vector.h"
#include "Window.h"
#include <iostream>

std::vector<Vec3> cube_points;
std::vector<Vec2> projected_points;
Vec3 cube_rotation {};
float fov_factor = 640.0f;

Engine::Engine(Framebuffer& fb, Window& window, UI& ui)
    : m_framebuffer(fb)
    , m_window(window)
    , m_ui(ui)
{
}

void Engine::setup()
{
    for (float x = -1; x < 1; x += 0.25) {
        for (float y = -1; y < 1; y += 0.25) {
            for (float z = -1; z < 1; z += 0.25) {
                cube_points.push_back(Vec3(x, y, z));
            }
        }
    }
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

    int delay = FRAME_TARGET_TIME - (SDL_GetTicks() - m_previous_frame_time);

    if (delay > 0 && delay <= FRAME_TARGET_TIME) {
        SDL_Delay(delay);
    }

    m_previous_frame_time = SDL_GetTicks();

    auto project = [](Vec3 p) {
        return Vec2(p.x / p.z * fov_factor, p.y / p.z * fov_factor);
    };

    cube_rotation.x += 0.02f;
    cube_rotation.y += 0.012f;
    cube_rotation.z += 0.018f;

    projected_points.clear();
    for (auto& p : cube_points) {

        auto transformed = p.rotate_x(cube_rotation.x);
        transformed = transformed.rotate_y(cube_rotation.y);
        transformed = transformed.rotate_z(cube_rotation.z);

        // Push points away from the camera.
        transformed.z += 5.0f;

        projected_points.push_back(project(transformed));
    }
}

void Engine::render()
{
    auto w = m_framebuffer.width();
    auto h = m_framebuffer.height();

    m_framebuffer.clear(0x333333FF);

    for (auto& p : projected_points) {
        auto centered_x = p.x + (w / 2.0f);
        auto centered_y = p.y + (h / 2.0f);

        m_framebuffer.draw_rect(centered_x, centered_y, 2, 2, 0xFFFF00FF);
    }
    m_window.render();
}
