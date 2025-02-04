# Set GLFW-specific options
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Disable GLFW examples")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable GLFW tests")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Disable GLFW documentation")

# Add GLFW submodule to the project
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../vendor/glfw ${CMAKE_BINARY_DIR}/glfw)
