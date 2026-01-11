add_library(LimitOrderBook
        OrderBook.cpp
        OrderBook.h
        Price.h
        Quantity.h
        OrderType.h
        Trade.h
        typedefs.h
)
target_compile_options(LimitOrderBook PUBLIC -Werror=switch -Wall -Wextra -Werror)
target_include_directories(LimitOrderBook PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
