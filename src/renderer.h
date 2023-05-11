#pragma once

#include "window.h"

class Renderer {
  public:
    Renderer(Window &window) : m_window(window){};

    auto draw_line(int x0, int y0, int x1, int y1, Color color) -> void;
    auto draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, Color color) -> void;
    void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, Color color);

  private:
    Window &m_window;
};
