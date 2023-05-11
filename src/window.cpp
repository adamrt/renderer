#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "window.h"

Window::Window(int width, int height) : m_width(width), m_height(height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        exit(EXIT_FAILURE);
    }

    m_window = SDL_CreateWindow(nullptr, 0, 0, m_width, m_height, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        fprintf(stderr, "Error creating SDL window.\n");
        exit(EXIT_FAILURE);
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        exit(EXIT_FAILURE);
    }

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
                                  m_width, m_height);

    m_color_buffer.resize(get_size());
}

Window::~Window() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Window::render() {
    int row_width = m_width * (int)sizeof(uint32_t);
    SDL_UpdateTexture(m_texture, nullptr, m_color_buffer.data(), row_width);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}

void Window::clear(Color color) {
    for (int i = 0; i < get_size(); i++) {
        m_color_buffer[i] = color.sdl();
    }
}

void Window::set_pixel(int x, int y, Color color) {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return;
    }
    m_color_buffer[(m_width * y) + x] = color.sdl();
}
