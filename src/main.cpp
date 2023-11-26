#include <iostream>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "AK.h"
#include "Camera.h"
#include "Engine.h"
#include "Framebuffer.h"
#include "UI.h"
#include "Window.h"

const bool FULLSCREEN = false;
constexpr i32 WINDOW_WIDTH = FULLSCREEN ? 2560 : 2560 / 2;
constexpr i32 WINDOW_HEIGHT = FULLSCREEN ? 1440 : 1440 / 2;

const i32 FB_SCALE = FULLSCREEN ? 2 : 1;
const i32 FB_WIDTH = ((WINDOW_WIDTH / 4) * 3) / FB_SCALE;
const i32 FB_HEIGHT = WINDOW_HEIGHT / FB_SCALE;
const f32 FB_ASPECT = (f32)FB_HEIGHT / FB_WIDTH;

constexpr u32 FPS = 60;

i32 main()
{
    Mesh mesh("res/drone.obj");
    Texture texture("res/drone.png");
    Model model(mesh, texture);

    Light light_b { Vec3(2.0f, 0.0f, 0.0f), Color::Blue };
    Light light_r { Vec3(-2.0f, 0.0f, 0.0f), Color::Red };

    Scene scene;
    scene.models.push_back(model);
    scene.lights.push_back(light_b);
    scene.lights.push_back(light_r);

    Camera camera { FB_ASPECT };
    Framebuffer framebuffer(camera, FB_WIDTH, FB_HEIGHT, FB_SCALE);
    Window window(framebuffer, WINDOW_WIDTH, WINDOW_HEIGHT, FULLSCREEN);
    UI ui { window, framebuffer, camera, scene };
    Engine engine(window, framebuffer, camera, ui, scene);

    engine.setup();
    engine.set_fps(FPS);
    while (engine.is_running()) {
        engine.process_input();
        engine.update();
        engine.render();
    }

    return 0;
}
