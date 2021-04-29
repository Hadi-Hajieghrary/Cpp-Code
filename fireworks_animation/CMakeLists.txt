cmake_minimum_required(VERSION 3.0.0)

cmake_minimum_required(VERSION 3.0.0)

add_definitions('--std=c++17')

project(fireworks_animation VERSION 0.1.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)

include_directories(${PROJECT_NAME} PUBLIC include )
add_executable(${PROJECT_NAME} main.cpp src/canvas.cpp)

target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image SDL2::GFX)


