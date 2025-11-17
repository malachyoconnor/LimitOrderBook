cmake_minimum_required(VERSION 3.28)
project(LimitOrderBook)

set(CMAKE_CXX_STANDARD 20)

add_executable(LimitOrderBook main.cpp
        Order.h
        Price.h
        Quantity.h
        typedefs.h
        Random.h
        OrderBook.cpp
        OrderBook.h)

target_compile_options(LimitOrderBook PUBLIC -Werror=switch)