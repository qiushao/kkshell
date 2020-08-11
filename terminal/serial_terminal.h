//
// Created by min on 2020/8/7.
//

#ifndef KKSHELL_SERIAL_TERMINAL_H
#define KKSHELL_SERIAL_TERMINAL_H

#include <QtSerialPort/QSerialPort>
#include "terminal/base_terminal.h"

struct SerialSettings {
    QString name;
    qint32 baudRate;
    QSerialPort::DataBits dataBits;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stopBits;
    QSerialPort::FlowControl flowControl;
};

class SerialTerminal : public BaseTerminal {
public:
    explicit SerialTerminal(const SerialSettings &settings, QWidget *parent);
    ~SerialTerminal() override;
public:
    void connect() override;
    void disconnect() override;

private:
    QSerialPort *serial_ = nullptr;
    SerialSettings settings_;
};


#endif //KKSHELL_SERIAL_TERMINAL_H
