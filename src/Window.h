#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "Framebuffer.h"
#include "UI.h"

constexpr int FPS = 60;
constexpr int FRAME_TARGET_TIME = (1000 / FPS);

class Window {
public:
    Window(Framebuffer& fb, UI& ui, int width, int height);
    ~Window();

    void render();

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
};
