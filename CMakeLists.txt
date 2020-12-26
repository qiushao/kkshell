cmake_minimum_required(VERSION 3.8)
project(kkshell LANGUAGES CXX)

set(PACKAGE_VERSION 1.0.0)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INSTALL_PREFIX "/usr")

add_subdirectory(sources)

include(cmake/deb.cmake)
include(CPack)