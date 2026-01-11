add_library(visualiser HistogramRenderer.h HistogramRenderer.cpp
        visualiser_utils.h)

target_link_libraries(visualiser PUBLIC raylib)
target_link_libraries(visualiser PRIVATE LimitOrderBook)
target_compile_definitions(visualiser PUBLIC FONT_LOCATION="${CMAKE_CURRENT_SOURCE_DIR}/Ubuntu_Mono.ttf")
target_compile_options(visualiser PUBLIC -Werror=switch -Wall -Wextra -Werror)

target_include_directories(visualiser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

