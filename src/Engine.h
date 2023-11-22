#pragma once

#include "AK.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Matrix.h"
#include "UI.h"
#include "Window.h"

class Engine {
public:
    Engine(Framebuffer& fb, Window& window, Camera& camera, UI& ui);

    void setup();
    void process_input();
    void update();
    void render();

    bool is_running(void) { return m_running; }

    void reset_orientation();

private:
    Framebuffer& m_framebuffer;
    Window& m_window;
    Camera& m_camera;
    UI& m_ui;

    u32 m_previous_frame_time = SDL_GetTicks();
    bool m_running = true;

    Mat4 m_projection_matrix;
};
