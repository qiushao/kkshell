cmake_minimum_required(VERSION 3.5)

project(kkshell LANGUAGES CXX C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(.)
include_directories(thirdparty/spdlog/spdlog-1.5.0/include)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INSTALL_PREFIX "/usr")

find_package(Qt5 COMPONENTS Core Widgets LinguistTools REQUIRED)

add_subdirectory(thirdparty)
add_subdirectory(common)
add_subdirectory(terminal)

set(TS_FILES resources/lang/kkshell_zh_CN.ts)
set(FORM_FILES resources/forms/mainwindow.ui resources/forms/settings.ui resources/forms/session_manager.ui)
set(QRC_FILES resources/resources.qrc)
set(SRC_FILES main.cpp mainwindow.cpp
        widget/command_button.cpp settings.cpp session_manager.cpp widget/edit_session_dialog.cpp
        widget/edit_command_group_dialog.cpp widget/edit_command_button_dialog.cpp
        )

add_executable(kkshell
        ${SRC_FILES}
        ${FORM_FILES}
        ${TS_FILES}
        ${QRC_FILES}
        )

set(THIRD_PARTY_LIBS simpleini)
set(PROJECT_LIBS config utils terminals)

target_link_libraries(kkshell PRIVATE Qt5::Core Qt5::Widgets ${THIRD_PARTY_LIBS} ${PROJECT_LIBS})

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

INSTALL(TARGETS kkshell
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

install(FILES resources/kkshell.desktop
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/)

install(FILES resources/images/kkshell.png
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/128x128/apps)
