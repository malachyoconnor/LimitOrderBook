add_library(uuid uuid.h)

target_include_directories(uuid
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(uuid PROPERTIES LINKER_LANGUAGE CXX)
