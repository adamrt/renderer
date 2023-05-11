#pragma once

#include <SDL2/SDL.h>
#include <cstdint>
#include <vector>

#include "color.h"
#include "imgui.h"

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

    // Move these. They are just for initial imgui import.
    ImGuiIO *m_io;
    int m_counter;
    ImVec4 m_clear_color = ImVec4(0.45, 0.55, 0.60, 1.00);
};
