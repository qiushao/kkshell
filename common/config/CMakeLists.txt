add_library(config config_manager.cpp config_manager.h)
target_link_libraries(config PRIVATE Qt5::Core)