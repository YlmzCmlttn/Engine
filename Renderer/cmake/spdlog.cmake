# Set spdlog-specific options
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "Disable spdlog examples")
set(SPDLOG_BUILD_TESTS OFF CACHE BOOL "Disable spdlog tests")
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "Disable spdlog benchmarks")

# Add spdlog submodule to the project
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../vendor/spdlog ${CMAKE_BINARY_DIR}/spdlog)