cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(ImGui LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gather all ImGui source files
file(GLOB IMGUI_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/../vendor/imgui/*.cpp
)

# Create a shared library for ImGui
add_library(ImGui SHARED ${IMGUI_SOURCES})

# Include the current directory for header files
target_include_directories(ImGui PUBLIC "${CMAKE_CURRENT_LIST_DIR}/../vendor/imgui")


# Set ImGui-specific options if needed
set(IMGUI_BUILD_EXAMPLES OFF CACHE BOOL "Disable ImGui examples")
set(IMGUI_BUILD_TESTS OFF CACHE BOOL "Disable ImGui tests")

# Add ImGui submodule to the project
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../vendor/imgui ${CMAKE_BINARY_DIR}/imgui)

# Link ImGui to your target
# Ensure to replace 'your_target_name' with the actual target name
# target_link_libraries(your_target_name PRIVATE ImGui)