#pragma once

#include <cstdint>
#include <vector>

#include "Color.h"
#include "UI.h"

int edge_function(int x0, int y0, int x1, int y1, int x2, int y2);

class Framebuffer {
public:
    Framebuffer(UI& ui, int width, int height);

    const std::vector<Color>& colorbuffer() { return m_colorbuffer; }

    int width() const { return m_width; }
    int height() const { return m_height; }
    float aspect() const { return (float)m_height / (float)m_width; }

    void render();
    void clear(Color color);
    void draw_pixel(int x, int y, Color color);
    void draw_grid(Color color);
    void draw_line(int x0, int y0, int x1, int y1, Color color);
    void draw_rect(int x, int y, int w, int h, Color color);
    void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, Color color);
    void draw_triangle_filled(int x0, int y0, int x1, int y1, int x2, int y2, Color color);

private:
    UI& m_ui;
    int m_width;
    int m_height;
    std::vector<Color> m_colorbuffer;
};
