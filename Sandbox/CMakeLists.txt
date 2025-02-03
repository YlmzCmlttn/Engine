#cmake -DCMAKE_CUDA_FLAGS="--gpu-architecture=sm_61" ..
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(SandBox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(../Renderer Renderer_Build)
file(GLOB SANDBOX_SOURCES src/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
macro(add_sandbox_example name)
add_executable(${name} ${SANDBOX_SOURCES})
target_compile_options(${name}  PRIVATE -Wall -Wextra -O3)
target_precompile_headers(${name} PRIVATE ../pch/pch.h)
target_link_libraries(${name} Renderer)
endmacro()

add_sandbox_example(SandBoxApp)
