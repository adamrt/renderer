cmake_minimum_required(VERSION 3.18)
project(
  Heretic
  VERSION     0.1
  DESCRIPTION "Final Fantasy Tactics Tooling"
  LANGUAGES   CXX)

# Using C++11 standard to allow building on PSVita.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)

add_executable(heretic src/main.cpp)
target_include_directories(heretic PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(heretic ${SDL2_LIBRARIES})
target_compile_options(heretic PRIVATE -Werror -Wall -Wextra -Wpedantic)
