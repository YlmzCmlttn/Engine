cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Engine LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ENGINE_SOURCES src/*.cpp)

add_library(
    Engine STATIC
    ${ENGINE_SOURCES}
)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/glad.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/glfw.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/spdlog.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/imgui.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/glm.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/assimp.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/stb.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/entt.cmake)
target_compile_definitions(Engine INTERFACE SPDLOG_COMPILED_LIB)
target_link_libraries(Engine PRIVATE glfw glad spdlog::spdlog ImGui glm::glm assimp stb EnTT::EnTT)
target_precompile_headers(Engine PRIVATE ../pch/pch.h)
target_include_directories(Engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include")

# Add debugging options
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(Engine PRIVATE -g -O0) # Add debug symbols and no optimization
endif()
