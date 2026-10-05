cmake_minimum_required(VERSION 3.16)
project(BloodCirculationGraphSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(circulation_core
    src/CirculatoryGraph.cpp
    src/Simulation.cpp
)

target_include_directories(circulation_core PUBLIC src)
target_compile_options(circulation_core PRIVATE -Wall -Wextra -pedantic)

add_executable(circulation_sim src/main.cpp)
target_link_libraries(circulation_sim PRIVATE circulation_core)
