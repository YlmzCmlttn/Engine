
# Add SPIRV-Cross as a subdirectory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../vendor/spirv-cross ${CMAKE_BINARY_DIR}/spirv-cross)

# Link against the SPIRV-Cross libraries, not the executable
# These are typical library targets provided by SPIRV-Cross
set(SPIRV_CROSS_LIBS spirv-cross-core spirv-cross-glsl spirv-cross-cpp)
