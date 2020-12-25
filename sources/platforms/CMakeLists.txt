if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PLATFORM linux)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(PLATFORM mac)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(PLATFORM windows)
else ()
    message(ERROR "not support ${CMAKE_SYSTEM_NAME}")
endif ()

message(STATUS "Configuring for ${CMAKE_SYSTEM_NAME}")

add_library(PlatformAdapter STATIC "")

target_sources(PlatformAdapter
        PRIVATE
        ${PLATFORM}/PlatformAdapter.cpp
        )

target_include_directories(PlatformAdapter
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )
