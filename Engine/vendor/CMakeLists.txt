cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(ImGui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gather all ImGui source files
file(GLOB IMGUI_SOURCES
    *.cpp
)

# Create a shared library for ImGui
add_library(ImGui SHARED ${IMGUI_SOURCES})

# Include the current directory for header files
target_include_directories(ImGui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# Link any necessary libraries here
# target_link_libraries(ImGui <dependencies>) 