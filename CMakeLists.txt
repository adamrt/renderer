cmake_minimum_required(VERSION 3.18)
project(
  Renderer
  VERSION     0.1
  DESCRIPTION "3D Software Renderer"
  LANGUAGES   CXX)

# Using C++11 standard to allow building on PSVita.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

find_package(SDL2 REQUIRED)

file(GLOB IMGUI_SOURCES "lib/imgui/*.h" "lib/imgui/*.cpp")
add_library(imgui ${IMGUI_SOURCES} lib/imgui/imgui.cpp)
target_include_directories(imgui PUBLIC ${SDL2_INCLUDE_DIRS})

add_executable(renderer
  src/main.cpp
  src/Camera.cpp
  src/Color.cpp
  src/Engine.cpp
  src/Framebuffer.cpp
  src/Matrix.cpp
  src/Mesh.cpp
  src/Triangle.cpp
  src/Texture.cpp
  src/UI.cpp
  src/Vector.cpp
  src/Window.cpp)
target_include_directories(renderer PRIVATE ${SDL2_INCLUDE_DIRS} lib/imgui/ lib/stb/)
target_link_libraries(renderer ${SDL2_LIBRARIES} imgui)
target_compile_options(renderer PRIVATE -Werror -Wall -Wextra -Wpedantic)
