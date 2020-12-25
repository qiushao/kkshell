set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Widgets SerialPort REQUIRED)

add_subdirectory(thirdparty)
add_subdirectory(utils)
add_subdirectory(platforms)
add_subdirectory(setting)

set(SRC_FILES
        main.cpp
        MainWindow.cpp
        widgets/session/SessionListWidget.cpp
        widgets/session/SessionCategoryEditor.cpp
        widgets/session/LocalSessionEditor.cpp
        widgets/session/SSHSessionEditor.cpp
        widgets/session/SerialSessionEditor.cpp
        widgets/terminal/BaseTerminal.cpp
        widgets/terminal/LocalTerminal.cpp
        widgets/terminal/SerialTerminal.cpp
        widgets/terminal/SSHTerminal.cpp
        widgets/command/CommandButton.cpp
        widgets/command/CommandToolBar.cpp
        widgets/command/CommandButtonEditor.cpp
        widgets/command/CommandCategoryEditor.cpp
        widgets/SettingDialog.cpp
        )

set(QRC_FILES resources/resources.qrc)

add_executable(${PROJECT_NAME}
        ${SRC_FILES}
        ${QRC_FILES}
        )

target_link_libraries(${PROJECT_NAME}
        Qt5::Core Qt5::Widgets Qt5::SerialPort
        RapidJSON Qt-AES
        setting kkterminalwidget
        )

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

install(FILES resources/kkshell.desktop
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications/)

install(FILES resources/images/kkshell.png
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/icons/hicolor/128x128/apps)
