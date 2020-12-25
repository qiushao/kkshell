add_library(setting STATIC "")

target_sources(setting
        PRIVATE
        SettingManager.cpp
        )

target_include_directories(setting
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(setting
        PUBLIC
        RapidJSON
        PlatformAdapter
        utils

        PRIVATE
        Qt5::Core
        )