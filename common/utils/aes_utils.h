//
// Created by mint on 2020/8/15.
//

#ifndef KKSHELL_AES_UTILS_H
#define KKSHELL_AES_UTILS_H


#include <string>

class AESUtils {
public:
    static std::string aes_encrypt(const std::string &in);
    static std::string aes_decrypt(const std::string &in);
};


#endif //KKSHELL_AES_UTILS_H
