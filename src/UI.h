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
#include "Scene.h"
#include "Vector.h"

enum class Projection {
    Perspective = 0,
    Orthographic = 1,
};

class UI {
public:
    UI(int ui_width, Scene& scene);
    ~UI();

    std::function<void()> update_camera;
    std::function<void()> orientation_event;

    void init(SDL_Window* w, SDL_Renderer* r, SDL_Texture* t, int fb_width, int fb_height);
    void render();
    void update();

    bool rotate = false;
    bool rotate_x = false;
    bool rotate_y = true;
    bool rotate_z = false;

    bool enable_fill_convention = true;
    bool enable_lighting = true;

    bool draw_texture = true;
    bool perspective_correction = true;
    bool draw_filled = false;
    ImVec4 fill_color = { 1.0f, 1.0f, 1.0f, 1.0f };
    ImVec4 ambient_light = { 1.0f, 1.0f, 1.0f, 1.0f };
    f32 ambient_strength = 0.45f;

    bool draw_empty = false;

    bool draw_wireframe = false;
    bool backface_culling = true;

    Projection projection = Projection::Perspective;
    f32 camera_phi = 0.0f;
    f32 camera_theta = 0.0f;

    int total_triangles = 0;
    int visible_triangles = 0;

private:
    SDL_Texture* m_texture = nullptr;
    int m_fb_width = 0;
    int m_fb_height = 0;
    int m_width = 0;
    ImGuiIO* m_io;
    Scene& m_scene;
};
