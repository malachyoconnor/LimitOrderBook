cmake_minimum_required(VERSION 3.28)
project(LimitOrderBook)

set(CMAKE_CXX_STANDARD 20)

add_library(src_lib
        typedefs.h
        Random.h
        OrderBook.cpp
        OrderBook.h
        Quantity.h
        Price.h
        Order.h)

target_include_directories(src_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
enable_testing()
add_subdirectory(test)

add_executable(LimitOrderBook main.cpp)
target_link_libraries(LimitOrderBook PUBLIC src_lib)

target_compile_options(src_lib PUBLIC -Werror=switch -Wall -Wextra -Werror)