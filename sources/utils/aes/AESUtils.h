//
// Created by qiushao on 2020/11/1.
//

#ifndef KKSHELL_AESUTILS_H
#define KKSHELL_AESUTILS_H

#include <QString>
#include <QCryptographicHash>
#include "qaesencryption.h"

class AESUtils {
public:
    static inline AESUtils *getInstance() {
        if (_instance == nullptr) {
            _instance = new AESUtils();
        }
        return _instance;
    }

private:
    static AESUtils *_instance;
    AESUtils();
    AESUtils(const AESUtils &) {};
    AESUtils &operator=(const AESUtils &) { return *this; }

public:
    QString encrypt(const QString &in);
    QString decrypt(const QString &in);

private:
    QAESEncryption *encryption;
    QString key;
    QString iv;
    QByteArray hashKey;
    QByteArray hashIV;
};


#endif //KKSHELL_AESUTILS_H
