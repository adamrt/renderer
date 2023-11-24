#pragma once

#include "AK.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Matrix.h"
#include "Scene.h"
#include "UI.h"
#include "Window.h"

constexpr int FPS = 60;
constexpr int FRAME_TARGET_TIME = (1000 / FPS);

class Engine {
public:
    Engine(Window& window, Framebuffer& fb, Camera& camera, UI& ui, Scene& scene);

    void setup();
    void process_input();
    void update();
    void render();

    bool is_running(void) { return m_running; }

    void reset_orientation();

private:
    Window& m_window;
    Framebuffer& m_framebuffer;
    Camera& m_camera;
    UI& m_ui;
    Scene& m_scene;

    u32 m_previous_frame_time = SDL_GetTicks();
    bool m_running = true;
    Mat4 m_projection_matrix;
};
