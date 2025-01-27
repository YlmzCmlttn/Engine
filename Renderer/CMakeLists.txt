cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(Renderer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3")

file(GLOB_RECURSE RENDERER_SOURCES src/*.cpp)

add_library(
    Renderer STATIC
    ${RENDERER_SOURCES}
)

target_precompile_headers(Renderer PRIVATE ../pch/pch.h)
target_include_directories(Renderer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")