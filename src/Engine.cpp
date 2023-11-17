#include "Engine.h"

Engine::Engine(Framebuffer& fb, Window& window)
    : m_framebuffer(fb)
    , m_window(window)
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
    m_framebuffer.draw_triangle_filled(10, 370, 450, 760, 680, 80, 0xFF0000FF);
    m_framebuffer.draw_triangle_filled(180, 50, 150, 1, 70, 180, 0x00FF00FF);
}

void Engine::render()
{
    m_window.render();
}
