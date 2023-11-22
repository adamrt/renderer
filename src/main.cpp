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

const i32 SCREEN_WIDTH = 1200;
const i32 SCREEN_HEIGHT = 800;

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

    UI ui { (SCREEN_WIDTH / 3), scene };
    Camera camera { ui };
    Framebuffer framebuffer(ui, (SCREEN_WIDTH / 3) * 2, SCREEN_HEIGHT);
    Window window(framebuffer, ui, SCREEN_WIDTH, SCREEN_HEIGHT);
    Engine engine(framebuffer, window, camera, ui, scene);

    engine.setup();
    while (engine.is_running()) {
        engine.process_input();
        engine.update();
        engine.render();
    }

    return 0;
}
