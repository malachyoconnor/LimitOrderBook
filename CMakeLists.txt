cmake_minimum_required(VERSION 3.28)
project(OrderBook)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(limitOrderBook) # LOB implementation
add_subdirectory(libs)           # raylib
add_subdirectory(visualiser)     # Visualisation implementation

add_executable(LOB main.cpp RandomOrderGenerator.h)
target_link_libraries(LOB PUBLIC LimitOrderBook)

add_executable(go_visualise visualise.cpp RandomOrderGenerator.h)
target_link_libraries(go_visualise PUBLIC visualiser LimitOrderBook raylib)
