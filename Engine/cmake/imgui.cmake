# Set ImGui-specific options if needed
set(IMGUI_BUILD_EXAMPLES OFF CACHE BOOL "Disable ImGui examples")
set(IMGUI_BUILD_TESTS OFF CACHE BOOL "Disable ImGui tests")

# Add ImGui submodule to the project
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../vendor/imgui ${CMAKE_BINARY_DIR}/imgui)

# Link ImGui to your target
# Ensure to replace 'your_target_name' with the actual target name
# target_link_libraries(your_target_name PRIVATE ImGui)