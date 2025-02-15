
file(GLOB STB_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/../vendor/stb/*.h
    ${CMAKE_CURRENT_LIST_DIR}/../vendor/stb/*.c
)


add_library(stb STATIC
    ${STB_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/../vendor/stb.cpp
)

target_include_directories(stb INTERFACE 
    ${CMAKE_CURRENT_LIST_DIR}/../vendor/stb
)