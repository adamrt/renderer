#pragma once

#include "window.h"

class Renderer {
  public:
    Renderer(Window &window) : m_window(window){};

    void draw_line(int x0, int y0, int x1, int y1, Color color);

  private:
    Window &m_window;
};
