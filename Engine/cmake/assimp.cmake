# Set Assimp-specific build options
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable Assimp tests")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Disable Assimp tools")
set(ASSIMP_INSTALL OFF CACHE BOOL "Disable Assimp installation step")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT ON CACHE BOOL "Build all importers by default")

# Add the Assimp submodule to the project.
# This will process the vendor/assimp CMakeLists.txt and build Assimp targets.
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../vendor/assimp ${CMAKE_BINARY_DIR}/assimp) 