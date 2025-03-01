# Disable tests for shaderc
set(SHADERC_SKIP_TESTS True)

# Add shaderc as a subdirectory
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../vendor/shaderc ${CMAKE_BINARY_DIR}/shaderc)
