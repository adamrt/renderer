#include "renderer.h"

auto Renderer::draw_line(int x0, int y0, int x1, int y1, Color color) -> void {
    int delta_x = (x1 - x0);
    int delta_y = (y1 - y0);

    int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

    float x_inc = (float)delta_x / (float)longest_side_length;
    float y_inc = (float)delta_y / (float)longest_side_length;

    auto current_x = (float)x0;
    auto current_y = (float)y0;

    for (int i = 0; i <= longest_side_length; i++) {
        m_window.set_pixel((int)round(current_x), (int)round(current_y), color);
        current_x += x_inc;
        current_y += y_inc;
    }
}
