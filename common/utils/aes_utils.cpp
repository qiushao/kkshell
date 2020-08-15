//
// Created by mint on 2020/8/15.
//

#include "aes_utils.h"
#include <string.h>
#include <QString>
#include <openssl/aes.h>

static const char *key = "just fo fun";

static bool _aes_encrypt(char *in, char *out) {
    if (!in || !key || !out)
    {
        return false;
    }

    AES_KEY aes;
    if (AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return false;
    }

    int len = strlen(in), en_len = 0;

    while (en_len < len)
    {
        AES_encrypt((unsigned char*)in, (unsigned char*)out, &aes);
        in	+= AES_BLOCK_SIZE;
        out += AES_BLOCK_SIZE;
        en_len += AES_BLOCK_SIZE;
    }

    return true;
}

static bool _aes_decrypt(char *in, char *out) {
    if (!in || !key || !out)
    {
        return false;
    }

    AES_KEY aes;
    if (AES_set_decrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return false;
    }

    int len = strlen(in), en_len = 0;
    while (en_len < len)
    {
        AES_decrypt((unsigned char*)in, (unsigned char*)out, &aes);
        in	+= AES_BLOCK_SIZE;
        out += AES_BLOCK_SIZE;
        en_len += AES_BLOCK_SIZE;
    }

    return true;
}


std::string AESUtils::aes_encrypt(const std::string &in) {
    if (in.empty()) {
        return std::string();
    }

    char data[128], encrypt_buffer[128];
    memset(data, 0, sizeof(data));
    memset(encrypt_buffer, 0, sizeof(encrypt_buffer));
    strcpy(data, in.c_str());
    _aes_encrypt(data, encrypt_buffer);

    QByteArray array(encrypt_buffer);
    QString str(array.toBase64());
    return str.toStdString();
}

std::string AESUtils::aes_decrypt(const std::string &in) {
    if (in.empty()) {
        return std::string();
    }

    char data[128], decrypt_buffer[128];
    memset(data, 0, sizeof(data));
    memset(decrypt_buffer, 0, sizeof(decrypt_buffer));
    QByteArray array(QByteArray::fromBase64(in.data()));
    memcpy(data, array.data(), array.size());
    _aes_decrypt(data, decrypt_buffer);
    return std::string(decrypt_buffer);
}

