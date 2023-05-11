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

auto Renderer::draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, Color color) -> void {
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

auto Renderer::draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, Color color)
    -> void {
    // We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
    if (y0 > y1) {
        std::swap(y0, y1);
        std::swap(x0, x1);
    }
    if (y1 > y2) {
        std::swap(y1, y2);
        std::swap(x1, x2);
    }
    if (y0 > y1) {
        std::swap(y0, y1);
        std::swap(x0, x1);
    }

    // Render the upper part of the triangle (flat-bottom)
    float inv_slope_1 = 0;
    float inv_slope_2 = 0;

    if (y1 - y0 != 0) {
        inv_slope_1 = (float)(x1 - x0) / abs((float)(y1 - y0));
    }
    if (y2 - y0 != 0) {
        inv_slope_2 = (float)(x2 - x0) / abs((float)(y2 - y0));
    }

    if (y1 - y0 != 0) {
        for (int y = y0; y <= y1; y++) {
            int x_start = (int)((float)x1 + (float)(y - y1) * inv_slope_1);
            int x_end = (int)((float)x0 + (float)(y - y0) * inv_slope_2);

            if (x_end < x_start) {
                std::swap(x_start, x_end); // swap if x_start is to the right of x_end
            }

            for (int x = x_start; x < x_end; x++) {
                // Draw our pixel with a solid color
                m_window.set_pixel(x, y, color);
            }
        }
    }

    // Render the bottom part of the triangle (flat-top)
    inv_slope_1 = 0;
    inv_slope_2 = 0;

    if (y2 - y1 != 0) {
        inv_slope_1 = (float)(x2 - x1) / abs((float)(y2 - y1));
    }
    if (y2 - y0 != 0) {
        inv_slope_2 = (float)(x2 - x0) / abs((float)(y2 - y0));
    }

    if (y2 - y1 != 0) {
        for (int y = y1; y <= y2; y++) {
            int x_start = (int)((float)x1 + (float)(y - y1) * inv_slope_1);
            int x_end = (int)((float)x0 + (float)(y - y0) * inv_slope_2);

            if (x_end < x_start) {
                std::swap(x_start, x_end); // swap if x_start is to the right of x_end
            }

            for (int x = x_start; x < x_end; x++) {
                // Draw our pixel with a solid color
                m_window.set_pixel(x, y, color);
            }
        }
    }
}
