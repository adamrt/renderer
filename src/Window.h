#pragma once

#include <functional>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "imgui_internal.h"

#include "Framebuffer.h"
#include "UI.h"

constexpr int FPS = 60;
constexpr int FRAME_TARGET_TIME = (1000 / FPS);

class Window {
public:
    Window(Framebuffer& fb, UI& ui, int width, int height);
    ~Window();

    void render();

    std::function<void()> update_camera;
    std::function<void()> orientation_event;

    int width() const { return m_width; }
    int height() const { return m_height; }

private:
    void draw_gui();

    Framebuffer& m_fb;
    UI& m_ui;

    int m_width;
    int m_height;

    SDL_Window* m_window = NULL;
    SDL_Renderer* m_renderer = NULL;
    SDL_Texture* m_texture = NULL;

    ImGuiIO* m_io;
};
