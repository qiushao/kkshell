add_library(RapidJSON INTERFACE)

target_include_directories(RapidJSON
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )