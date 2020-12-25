//
// Created by qiushao on 2020/11/1.
//

#include "AESUtils.h"

AESUtils *AESUtils::_instance = nullptr;

QString AESUtils::encrypt(const QString &in) {
    QByteArray encodeText = encryption->encode(in.toLocal8Bit(), hashKey, hashIV);
    return QString(encodeText.toBase64());
}

QString AESUtils::decrypt(const QString &in) {
    QByteArray encodeText(QByteArray::fromBase64(in.toLocal8Bit()));
    QByteArray decodeText = encryption->decode(encodeText, hashKey, hashIV);
    QString decodedString = QString(encryption->removePadding(decodeText));
    return decodedString;
}

AESUtils::AESUtils() {
    encryption = new QAESEncryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    key = ("qiushao-kkshell");
    iv = ("qiushao-kkshell");

    hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);
}
