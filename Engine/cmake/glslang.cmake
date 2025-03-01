# Add glslang as a subdirectory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../vendor/glslang ${CMAKE_BINARY_DIR}/glslang)

# Use the library targets provided by glslang
set(GLSLANG_LIBS glslang SPIRV)