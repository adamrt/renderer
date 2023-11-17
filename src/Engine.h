#pragma once

#include "Framebuffer.h"
#include "Window.h"

class Engine {
public:
    Engine(Framebuffer& fb, Window& window);

    void setup();
    void process_input();
    void update();
    void render();

    bool is_running(void) { return m_running; }

private:
    Framebuffer& m_framebuffer;
    Window& m_window;

    uint32_t m_previous_frame_time = SDL_GetTicks();
    bool m_running = true;
};
