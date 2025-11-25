add_library(visualiser HistogramRenderer.h HistogramRenderer.cpp)

target_link_libraries(visualiser PUBLIC raylib)
target_compile_options(visualiser PUBLIC -Werror=switch -Wall -Wextra -Werror)

target_include_directories(visualiser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

