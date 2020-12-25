add_library(Qt-AES STATIC "")

target_sources(Qt-AES
        PRIVATE
        qaesencryption.cpp
        )

target_include_directories(Qt-AES
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(Qt-AES
        PRIVATE
        Qt5::Core
        )