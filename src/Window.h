#pragma once

#include <cstdint>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include "imgui_internal.h"

#include "Framebuffer.h"

constexpr int FPS = 60;
constexpr int FRAME_TARGET_TIME = (1000 / FPS);

class Window {
public:
    Window(Framebuffer& fb, int width, int height);
    ~Window();

    void render();

    int get_width() { return m_width; }
    int get_height() { return m_height; }

private:
    void update_gui();

    Framebuffer& m_fb;
    int m_width;
    int m_height;

    SDL_Window* m_window = NULL;
    SDL_Renderer* m_renderer = NULL;
    SDL_Texture* m_texture = NULL;

    ImGuiIO* m_io;
};
