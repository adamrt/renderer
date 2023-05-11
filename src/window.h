#pragma once

#include <SDL2/SDL.h>
#include <cstdint>
#include <vector>

#include "color.h"

class Window {
  public:
    Window(int width, int height);
    ~Window();

    auto render() -> void;
    auto clear(uint32_t color) -> void;
    auto set_pixel(int x, int y, uint32_t color) -> void;

    [[nodiscard]] auto get_width() const -> int { return m_width; }
    [[nodiscard]] auto get_height() const -> int { return m_height; }
    [[nodiscard]] auto get_size() const -> int { return m_width * m_height; }

  private:
    int m_width;
    int m_height;
    std::vector<uint32_t> m_color_buffer;

    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    SDL_Texture *m_texture = nullptr;
};
