add_library(stb STATIC
    ${CMAKE_CURRENT_LIST_DIR}/../vendor/stb/stb.cpp
)

target_include_directories(stb INTERFACE 
    ${CMAKE_CURRENT_LIST_DIR}/../vendor/stb
)