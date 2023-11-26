#pragma once

#include "AK.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Matrix.h"
#include "Scene.h"
#include "UI.h"
#include "Window.h"

class Engine {
public:
    Engine(Window& window, Framebuffer& fb, Camera& camera, UI& ui, Scene& scene);

    void setup();
    void process_input();
    void update();
    void render();

    void set_fps(u32 fps) { m_target_fps = fps; }
    f32 target_frame_time() const { return (f32)1000.0 / m_target_fps; }

    bool is_running(void) { return m_running; }

    void reset_orientation();

private:
    Window& m_window;
    Framebuffer& m_framebuffer;
    Camera& m_camera;
    UI& m_ui;
    Scene& m_scene;

    u32 m_target_fps;
    u32 m_previous_frame_time = SDL_GetTicks();
    bool m_running = true;
    Mat4 m_projection_matrix;
};
