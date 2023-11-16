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

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
const int BUFFER_WIDTH = 800;
const int BUFFER_HEIGHT = 600;

int main()
{
    Framebuffer framebuffer(BUFFER_WIDTH, BUFFER_HEIGHT);
    Window window(framebuffer, SCREEN_WIDTH, SCREEN_HEIGHT);
    Engine engine(framebuffer, window);

    while (engine.is_running()) {
        engine.process_input();
        engine.update();
        engine.render();
    }

    return 0;
}
