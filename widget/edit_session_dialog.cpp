//
// Created by mint on 2020/8/11.
//

#include "edit_session_dialog.h"
#include <QDebug>
#include <QSerialPortInfo>
#include "common/config/config_manager.h"

EditSessionDialog::EditSessionDialog(const std::string &sessionName, const std::string &sessionType, QWidget *parent) : NewSessionDialog(sessionType, parent) {
    setWindowTitle("Session Edit");
    isEdit = true;
    oldSessionName = sessionName;

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
    ConfigManager *conf = ConfigManager::getInstance();
    sshHostEdit->setText(conf->getCString(sessionName.c_str(), "host"));
    sshPortEdit->setText(conf->getCString(sessionName.c_str(), "port"));
    sshUserEdit->setText(conf->getCString(sessionName.c_str(), "user"));
    sshPasswdEdit->setText(conf->getCString(sessionName.c_str(), "passwd"));
}

void EditSessionDialog::editSerialSession(const std::string &sessionName) {
    ConfigManager *conf = ConfigManager::getInstance();
    serialDevComboBox->setCurrentText(conf->getCString(sessionName.c_str(), "dev"));
    serialBaudRateComboBox->setCurrentText(conf->getCString(sessionName.c_str(), "baudRate"));
    serialDataBitsComboBox->setCurrentText(conf->getCString(sessionName.c_str(), "dataBits"));
    serialStopBitsComboBox->setCurrentText(conf->getCString(sessionName.c_str(), "stopBits"));
    serialParityComboBox->setCurrentText(conf->getCString(sessionName.c_str(), "parity"));
    serialFlowControlComboBox->setCurrentText(conf->getCString(sessionName.c_str(), "flowControl"));
}


