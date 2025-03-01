# Add SPIRV-Tools as a subdirectory
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../vendor/spirv-headers ${CMAKE_BINARY_DIR}/spirv-headers)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../vendor/spirv-tools ${CMAKE_BINARY_DIR}/spirv-tools)

#set(SPIRV-Headers_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../vendor/spirv-headers)
# Link against the SPIRV-Tools libraries
set(SPIRV_TOOLS_LIBS SPIRV-Tools-opt SPIRV-Tools)
