cmake_minimum_required(VERSION 3.28)
project(LimitOrderBook)

set(CMAKE_CXX_STANDARD 20)

add_executable(LimitOrderBook main.cpp
        typedefs.h
        Random.h
        OrderBook.cpp
        OrderBook.h
        Quantity.h
        Price.h
        Order.h
)

target_compile_options(LimitOrderBook PUBLIC -Werror=switch -Wall -Wextra -Werror)