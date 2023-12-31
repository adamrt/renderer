#pragma once

#include <functional>
#include <iostream>

#include <SDL.h>
#include <SDL_render.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "imgui_internal.h"

#include "AK.h"
#include "Framebuffer.h"
#include "Scene.h"
#include "Vector.h"
#include "Window.h"

class UI {
public:
    UI(Window& window, Framebuffer& framebuffer, Camera& camera, Scene& scene);
    ~UI();

    std::function<void()> orientation_event;

    i32 width() const;

    void update();
    void render();

    bool rotate { false };
    bool rotate_x { false };
    bool rotate_y { true };
    bool rotate_z { false };

    bool draw_texture { true };
    bool draw_filled { false };
    bool draw_normals { false };
    ImVec4 fill_color { 1.0f, 1.0f, 1.0f, 1.0f };

    bool draw_empty { false };

    bool draw_wireframe { false };
    bool backface_culling { true };

    int total_triangles {};
    int visible_triangles {};

private:
    Window& m_window;
    Framebuffer& m_framebuffer;
    Camera& m_camera;
    Scene& m_scene;

    ImGuiIO* m_io { nullptr };
};
