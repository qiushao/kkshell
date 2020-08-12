//
// Created by mint on 2020/8/11.
//

#include "edit_session_dialog.h"
#include <QDebug>
#include <QSerialPortInfo>

EditSessionDialog::EditSessionDialog(const std::string &sessionName, const std::string &sessionType, QWidget *parent) : NewSessionDialog(sessionType, parent) {
    setWindowTitle("Session Edit");

    if (sessionType == "serial") {
        editSerialSession(sessionName);
    } else if (sessionType == "ssh") {
        editSSHSession(sessionName);
    } else {
        qDebug() << "session type error" << endl;
    }

    sessionNameEidt->setText(sessionName.c_str());
}

EditSessionDialog::~EditSessionDialog() {

}

void EditSessionDialog::editSSHSession(const std::string &sessionName) {

}

void EditSessionDialog::editSerialSession(const std::string &sessionName) {

}


