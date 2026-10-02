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

option(BUILD_VISUALIZER "Build the SFML visual simulator" ON)
if(BUILD_VISUALIZER)
    include(FetchContent)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SFML)

    add_executable(circulation_visual
        src/main_visual.cpp
        src/Visualization.cpp
    )
    target_link_libraries(circulation_visual PRIVATE circulation_core sfml-graphics sfml-window sfml-system)
    target_compile_options(circulation_visual PRIVATE -Wall -Wextra -pedantic)
endif()
