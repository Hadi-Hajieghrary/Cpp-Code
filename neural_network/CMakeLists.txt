cmake_minimum_required(VERSION 3.0.0)
project(neural_network VERSION 0.1.0)
add_definitions('--std=c++17')

set (CMAKE_EXE_LINKER_FLAGS -pthread )

include_directories(${PROJECT_NAME} PUBLIC include )
add_executable(${PROJECT_NAME} main.cpp src/perceptron.cpp src/exceptions.cpp src/ml_perceptron.cpp src/utilities.cpp)


find_package (Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries (${PROJECT_NAME} Eigen3::Eigen)

add_definitions(-std=c++17 -DEIGEN_MATRIX_PLUGIN="Matrix_initializer_list.h" -DEIGEN_ARRAY_PLUGIN="Array_initializer_list.h")


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
