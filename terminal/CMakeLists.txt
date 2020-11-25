find_package(Qt5 COMPONENTS Widgets SerialPort REQUIRED)
add_library(terminals base_terminal.cpp base_terminal.h serial_terminal.cpp serial_terminal.h local_terminal.cpp local_terminal.h ssh_terminal.cpp ssh_terminal.h)
target_link_libraries(terminals Qt5::Widgets Qt5::SerialPort kkterminalwidget config)
