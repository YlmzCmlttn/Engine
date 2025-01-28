cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(Renderer LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE RENDERER_SOURCES src/*.cpp)

add_library(
    Renderer STATIC
    ${RENDERER_SOURCES}
)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/glad.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/glfw.cmake)
target_link_libraries(Renderer PRIVATE glfw glad)
target_precompile_headers(Renderer PRIVATE ../pch/pch.h)
target_include_directories(Renderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")