cmake_minimum_required(VERSION 3.28)
project(LimitOrderBook)

set(CMAKE_CXX_STANDARD 23)

add_library(LimitOrderBook
        OrderBook.cpp
        OrderBook.h
        Price.h
        Quantity.h
        OrderType.h
        Trade.h
        Random.h
        typedefs.h
)
target_compile_options(LimitOrderBook PUBLIC -Werror=switch -Wall -Wextra -Werror)

add_executable(LOB main.cpp)
target_link_libraries(LOB PUBLIC LimitOrderBook)


add_executable(visualiser visualiser.cpp)
add_subdirectory(libs)
target_link_libraries(visualiser PUBLIC LimitOrderBook raylib)