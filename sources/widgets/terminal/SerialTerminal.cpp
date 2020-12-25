//
// Created by qiushao on 2020/8/7.
//

#include "qtermwidget.h"
#include <unistd.h>
#include <QDebug>
#include <QtWidgets/QMessageBox>
#include "SerialTerminal.h"

SerialTerminal::SerialTerminal(const Session &session, QWidget *parent) : BaseTerminal(parent) {
    _session = session;

    _serial = new QSerialPort(this);
    // 把在终端的输入传给串口
    QObject::connect(this, &QTermWidget::sendData, [this](const char *data, int size) {
        _serial->write(data);
    });

    // 把串口传过来的数据传给终端
    QObject::connect(_serial, &QSerialPort::readyRead, [this]() {
        const QByteArray data = _serial->readAll();
        write(this->getPtySlaveFd(), data.data(), data.size());
    });

    // 串口发生错误时的回调处理
    QObject::connect(_serial, &QSerialPort::errorOccurred, this, &SerialTerminal::handleError);

    // Here we start an empty pty.
    this->startTerminalTeletype();
}

SerialTerminal::~SerialTerminal() {
    delete _serial;
}

void SerialTerminal::connect() {
    _serial->setPortName(_session.dev.c_str());
    _serial->setBaudRate(_session.baudRate);
    _serial->setDataBits(static_cast<QSerialPort::DataBits>(_session.dataBits));
    _serial->setParity(static_cast<QSerialPort::Parity>(_session.parity));
    _serial->setStopBits(static_cast<QSerialPort::StopBits>(_session.stopBits));
    _serial->setFlowControl(static_cast<QSerialPort::FlowControl>(_session.flowControl));
    if (_serial->open(QIODevice::ReadWrite)) {
        qDebug() << "open serial " << _session.sessionName.c_str() << " sucess" << endl;
    } else {
        QMessageBox::critical(this, tr("Error"), _serial->errorString());
    }

    _connect = true;
}

void SerialTerminal::disconnect() {
    _connect = false;
    if (_serial->isOpen()) {
        _serial->close();
    }
}

void SerialTerminal::handleError(QSerialPort::SerialPortError error) {
    if (error != QSerialPort::SerialPortError::NoError) {
        emit requestDisconnect(this);
    }
}
