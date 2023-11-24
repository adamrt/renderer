#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "Framebuffer.h"

class Window {
public:
    Window(Framebuffer& framebuffer, i32 width, i32 height);
    ~Window();

    void update_texture();
    void render();

    int width() const { return m_width; }
    int height() const { return m_height; }

    SDL_Window* sdl_window() const { return m_window; }
    SDL_Renderer* sdl_renderer() const { return m_renderer; }
    SDL_Texture* sdl_texture() const { return m_texture; }

private:
    Framebuffer& m_framebuffer;
    int m_width;
    int m_height;

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_texture = nullptr;
};
