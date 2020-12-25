add_library(utils STATIC "")

target_sources(utils
        PRIVATE
        file/FileUtils.cpp
        aes/AESUtils.cpp
        )

target_include_directories(utils
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/file
        ${CMAKE_CURRENT_SOURCE_DIR}/aes
        )

target_link_libraries(utils
        PUBLIC
        Qt-AES

        PRIVATE
        Qt5::Core
        )