cmake_minimum_required(VERSION 3.0.0)

add_definitions('--std=c++17')
set (CMAKE_EXE_LINKER_FLAGS -pthread )

project(drawing_fractal VERSION 0.1.0)


include_directories(${PROJECT_NAME} PUBLIC include )
add_executable(${PROJECT_NAME} main.cpp src/bitmap.cpp)

target_link_libraries( ${PROJECT_NAME} )
