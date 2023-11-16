#include "Engine.h"

Engine::Engine(Framebuffer& fb, Window& window)
    : m_framebuffer(fb)
    , m_window(window)
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
    int x = rand() % (m_framebuffer.width() + 1);
    int y = rand() % (m_framebuffer.height() + 1);
    m_framebuffer.draw_pixel(x, y, 0xFF0000FF);
}

void Engine::render()
{
    m_window.render();
}
