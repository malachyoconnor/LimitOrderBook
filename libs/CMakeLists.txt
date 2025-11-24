cmake_minimum_required(VERSION 3.28)

add_library(raylib STATIC IMPORTED GLOBAL)

set_target_properties(raylib PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libraylib.a"
)

target_include_directories(raylib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(raylib INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/raylib.h")
