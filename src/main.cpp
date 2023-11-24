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

const i32 WINDOW_WIDTH = 1200;
const i32 WINDOW_HEIGHT = 800;
const i32 FB_WIDTH = (WINDOW_WIDTH / 3) * 2;
const i32 FB_HEIGHT = WINDOW_HEIGHT;
const f32 FB_ASPECT = FB_WIDTH / (f32)FB_HEIGHT;

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
    Framebuffer framebuffer(camera, FB_WIDTH, FB_HEIGHT);
    Window window(framebuffer, WINDOW_WIDTH, WINDOW_HEIGHT);
    UI ui { window, framebuffer, camera, scene };
    Engine engine(window, framebuffer, camera, ui, scene);

    engine.setup();
    while (engine.is_running()) {
        engine.process_input();
        engine.update();
        engine.render();
    }

    return 0;
}
