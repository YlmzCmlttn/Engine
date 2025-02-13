#cmake -DCMAKE_CUDA_FLAGS="--gpu-architecture=sm_61" ..
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(Editor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(../Engine Engine_Build)
file(GLOB_RECURSE EDITOR_SOURCES src/*.cpp )
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
macro(add_editor_example name)
add_executable(${name} ${EDITOR_SOURCES})
target_compile_options(${name}  PRIVATE -Wall -Wextra -O3)
target_precompile_headers(${name} PRIVATE ../pch/pch.h)
target_link_libraries(${name} Engine ImGui glm::glm stb glad EnTT::EnTT)
endmacro()

add_editor_example(EditorApp)
