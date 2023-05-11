#include <SDL2/SDL.h>
#include <cstdio>

#include "color.h"
#include "renderer.h"
#include "window.h"

constexpr int FPS = 30;
constexpr int FrameTargetTime = (1000 / FPS);

constexpr int WindowWidth = 400;
constexpr int WindowHeight = 400;

auto main() -> int {
    auto window = Window(WindowWidth, WindowHeight);
    auto renderer = Renderer(window);

    auto is_running = true;
    auto previous_frame_time = SDL_GetTicks();

    while (is_running) {

        // Process Input
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                    break;
                }
            }
        }

        // Update
        //
        // Wait some time until the reach the target frame time in
        // milliseconds. Only delay execution if we are running too
        // fast.
        int time_to_wait = FrameTargetTime - (int)(SDL_GetTicks() - previous_frame_time);
        if (time_to_wait > 0 && time_to_wait <= FrameTargetTime) {
            SDL_Delay(time_to_wait);
        }
        previous_frame_time = SDL_GetTicks();

        // Render
        window.clear(Colors::Black);
        renderer.draw_line(0, 0, WindowWidth, WindowHeight, Colors::Green);
        renderer.draw_filled_triangle(WindowWidth, 0, WindowWidth / 2, WindowHeight, 0,
                                      WindowHeight / 2, Colors::White);
        renderer.draw_triangle(0, 0, WindowWidth, WindowHeight / 2, WindowWidth / 3, WindowHeight,
                               Colors::Red);
        window.render();
    }

    return 0;
}
