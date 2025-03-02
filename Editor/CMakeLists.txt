if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Editor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Engine subdirectory
add_subdirectory(../Engine Engine_Build)

# Gather source files and include directories
file(GLOB_RECURSE EDITOR_SOURCES src/*.cpp)
file(GLOB_RECURSE EDITOR_HEADERS src/*.h)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${EDITOR_SOURCES} ${EDITOR_HEADERS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# Macro to add an editor example executable with platform- and config-specific compile options
macro(add_editor_example name)
  add_executable(${name} ${EDITOR_SOURCES} ${EDITOR_HEADERS})

  # For MSVC (Windows), apply /W4 for warnings always, and /O2 for optimization in Release builds only.
  # For non-MSVC compilers (GCC/Clang), apply -Wall and -Wextra always, and -O3 in Release builds only.
  target_compile_options(${name} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /utf-8>
    $<$<AND:$<CXX_COMPILER_ID:MSVC>,$<CONFIG:Release>>:/O2>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra>
    $<$<AND:$<NOT:$<CXX_COMPILER_ID:MSVC>>,$<CONFIG:Release>>:-O3>
  )
  # Setup precompiled headers (adjust the path as needed)
  target_precompile_headers(${name} PRIVATE ../pch/pch.h)

  # Link required libraries
  target_link_libraries(${name} Engine ImGui glm::glm stb glad EnTT::EnTT assimp)
endmacro()

# Create the EditorApp executable
add_editor_example(EditorApp)
