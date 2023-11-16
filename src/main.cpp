#include <cstdint>
#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int ROW_WIDTH = SCREEN_WIDTH * sizeof(uint32_t);

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        exit(EXIT_FAILURE);
    }

    auto window = SDL_CreateWindow(NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Error creating SDL window.\n");
        exit(EXIT_FAILURE);
    }

    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "Error creating SDL renderer.\n");
        exit(EXIT_FAILURE);
    }

    auto texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!texture) {
        fprintf(stderr, "Error creating SDL texture.\n");
        exit(EXIT_FAILURE);
    }

    auto max = SCREEN_WIDTH * SCREEN_HEIGHT;

    std::vector<uint32_t> color_buffer(SCREEN_WIDTH * SCREEN_HEIGHT);

    auto is_running = true;

    while (is_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
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

        int pixel = rand() % (max + 1);
        color_buffer[pixel] = 0xFFFF0000;

        SDL_UpdateTexture(texture, NULL, &color_buffer[0], ROW_WIDTH);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
