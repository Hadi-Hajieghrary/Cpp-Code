cmake_minimum_required(VERSION 3.0.0)
project(neural_network VERSION 0.1.0)

add_definitions(-std=c++17 -DEIGEN_MATRIX_PLUGIN="Matrix_initializer_list.h" -DEIGEN_ARRAY_PLUGIN="Array_initializer_list.h")


set (CMAKE_EXE_LINKER_FLAGS -pthread )

add_executable(${PROJECT_NAME} main.cpp src/perceptron.cpp src/exceptions.cpp src/ml_perceptron.cpp src/utilities.cpp)

find_package(ROOT 6.16 CONFIG REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

include_directories(${PROJECT_NAME} PUBLIC include ${ROOT_INCLUDE_DIRS})


target_link_libraries (${PROJECT_NAME} PUBLIC ROOT::Core ROOT::Gui ROOT::Hist ROOT::Gpad Eigen3::Eigen)

#ROOT::Core ROOT::Gpad ROOT::Graf3d ROOT::Graf ROOT::Hist ROOT::Imt ROOT::MathCore ROOT::Matrix
#ROOT::MultiProc ROOT::Net ROOT::Physics ROOT::Postscript ROOT::RIO ROOT::ROOTDataFrame ROOT::ROOTVecOps
#ROOT::Rint ROOT::Thread ROOT::TreePlayer ROOT::Tree

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
