#include <cstdint>
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "Framebuffer.h"
#include "Window.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const int BUFFER_WIDTH = 800;
const int BUFFER_HEIGHT = 600;

int main()
{
    Framebuffer framebuffer(BUFFER_WIDTH, BUFFER_HEIGHT);
    Window window(framebuffer, SCREEN_WIDTH, SCREEN_HEIGHT);

    auto is_running = true;

    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

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

        int x = rand() % (BUFFER_WIDTH + 1);
        int y = rand() % (BUFFER_WIDTH + 1);
        framebuffer.draw_pixel(x, y, 0xFF0000FF);

        window.render();
    }

    return 0;
}
