find_package(OpenSSL REQUIRED)
add_library(utils aes_utils.cpp)
target_link_libraries(utils PRIVATE Qt5::Core OpenSSL::Crypto OpenSSL::SSL)