cmake_minimum_required(VERSION 3.28)
project(LimitOrderBook)

set(CMAKE_CXX_STANDARD 23)

add_executable(LimitOrderBook
        main.cpp
        Price.h
        Quantity.h
        OrderType.h
        OrderBook.cpp
        OrderBook.h
        Trade.h
        typedefs.h
        Random.h
)

target_compile_options(LimitOrderBook PUBLIC -Werror=switch -Wall -Wextra -Werror)