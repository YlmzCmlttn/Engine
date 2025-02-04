# Add the GLAD source directory


add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../vendor/glad/cmake ${CMAKE_BINARY_DIR}/glad)
glad_add_library(glad REPRODUCIBLE API gl:core=4.6)
# Add any additional GLAD-specific options here (if needed)
