#include <cstdint>
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "Engine.h"
#include "Framebuffer.h"
#include "Window.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

int main()
{
    Framebuffer framebuffer((SCREEN_WIDTH / 3) * 2, SCREEN_HEIGHT);
    Window window(framebuffer, SCREEN_WIDTH, SCREEN_HEIGHT);
    Engine engine(framebuffer, window);

    while (engine.is_running()) {
        engine.process_input();
        engine.update();
        engine.render();
    }

    return 0;
}
