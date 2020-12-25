//
// Created by qiushao on 2020/8/7.
//

#ifndef KKSHELL_SERIAL_TERMINAL_H
#define KKSHELL_SERIAL_TERMINAL_H

#include <QtSerialPort/QSerialPort>
#include "BaseTerminal.h"
#include "setting/Settings.h"

class SerialTerminal : public BaseTerminal {
public:
    explicit SerialTerminal(const Session &session, QWidget *parent);
    ~SerialTerminal() override;
    void connect() override;
    void disconnect() override;

private:
    void handleError(QSerialPort::SerialPortError error);
    QSerialPort *_serial = nullptr;
    Session _session;
};


#endif //KKSHELL_SERIAL_TERMINAL_H
