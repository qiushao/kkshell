//
// Created by min on 2020/8/7.
//

#include <qtermwidget.h>
#include <unistd.h>
#include <QDebug>
#include <QtWidgets/QMessageBox>
#include "serial_terminal.h"

SerialTerminal::SerialTerminal(const SerialSettings &settings, QWidget *parent) : BaseTerminal(parent) {
    settings_ = settings;

    serial_ = new QSerialPort(this);
    // 把在终端的输入传给串口
    QObject::connect(this, &QTermWidget::sendData, [this](const char *data, int size) {
        serial_->write(data);
    });

    // 把串口传过来的数据传给终端
    QObject::connect(serial_, &QSerialPort::readyRead, [this]() {
        const QByteArray data = serial_->readAll();
        write(this->getPtySlaveFd(), data.data(), data.size());
    });

    // 串口发生错误时的回调处理
    QObject::connect(serial_, &QSerialPort::errorOccurred, this, &SerialTerminal::handleError);

    // Here we start an empty pty.
    this->startTerminalTeletype();
}

SerialTerminal::~SerialTerminal() {
    delete serial_;
}

void SerialTerminal::connect() {
    serial_->setPortName(settings_.name);
    serial_->setBaudRate(settings_.baudRate);
    serial_->setDataBits(settings_.dataBits);
    serial_->setParity(settings_.parity);
    serial_->setStopBits(settings_.stopBits);
    serial_->setFlowControl(settings_.flowControl);
    if (serial_->open(QIODevice::ReadWrite)) {
        qDebug() << "open serial " << settings_.name << " sucess" << endl;
    } else {
        QMessageBox::critical(this, tr("Error"), serial_->errorString());
    }

    connect_ = true;
}

void SerialTerminal::disconnect() {
    connect_ = false;
    if (serial_->isOpen()) {
        serial_->close();
    }
}

void SerialTerminal::handleError(QSerialPort::SerialPortError error) {
    if (error != QSerialPort::SerialPortError::NoError) {
        emit requestDisconnect(this);
    }
}
