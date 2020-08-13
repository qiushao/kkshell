//
// Created by mint on 2020/8/12.
//

#include "new_session_dialog.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMessageBox>
#include "common/config/config_manager.h"

NewSessionDialog::NewSessionDialog(const std::string &sessionType, QWidget *parent): QDialog(parent) {
    setWindowTitle("New Session");
    initViews(sessionType);
}

NewSessionDialog::~NewSessionDialog() {

}

void NewSessionDialog::initViews(const std::string &sessionType) {
    mainLayout = new QVBoxLayout();

    sessionTypeLayout = new QHBoxLayout();
    sessionTypeLabel = new QLabel("Session Type:");
    sessionTypeComboBox = new QComboBox();
    sessionTypeComboBox->addItem("ssh", "ssh");
    sessionTypeComboBox->addItem("serial", "serial");
    sessionTypeComboBox->addItem("local", "local");
    sessionTypeLayout->addWidget(sessionTypeLabel);
    sessionTypeLayout->addWidget(sessionTypeComboBox);
    mainLayout->addLayout(sessionTypeLayout);

    sessionNameLayout = new QHBoxLayout();
    sessionNameLabel = new QLabel("Session Name:");
    sessionNameEidt = new QLineEdit();
    sessionNameLayout->addWidget(sessionNameLabel);
    sessionNameLayout->addWidget(sessionNameEidt);
    mainLayout->addLayout(sessionNameLayout);

    setLayout(mainLayout);

    if (sessionType == "local") {
        sessionTypeComboBox->setCurrentText("local");
        sessionTypeComboBox->setEnabled(false);
    } else if (sessionType == "serial") {
        sessionTypeComboBox->setCurrentText("serial");
        sessionTypeComboBox->setEnabled(false);
        newSerialSessionLayout();
        mainLayout->addLayout(serialSessionLayout);
    } else if (sessionType == "ssh") {
        sessionTypeComboBox->setCurrentText("ssh");
        sessionTypeComboBox->setEnabled(false);
        newSSHSessionLayout();
        mainLayout->addLayout(sshSessionLayout);
    } else {
        qDebug() << "session type error" << endl;
    }

    newButtonLayout();
    mainLayout->addLayout(buttonLayout);
}

void NewSessionDialog::newSerialSessionLayout() {
    if (serialSessionLayout != nullptr) {
        return;
    }
    serialSessionLayout = new QVBoxLayout();

    serialDevLayout = new QHBoxLayout();
    serialDevLabel = new QLabel("Serial Device:");
    serialDevComboBox = new QComboBox();
    serialDevLayout->addWidget(serialDevLabel);
    serialDevLayout->addWidget(serialDevComboBox);
    const auto devices = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &dev : devices) {
        serialDevComboBox->addItem(dev.portName(), dev.portName());
    }
    serialSessionLayout->addLayout(serialDevLayout);

    serialBaudRateLayout = new QHBoxLayout();
    serialBaudRateLabel = new QLabel("Baud Rate:");
    serialBaudRateComboBox = new QComboBox();
    serialBaudRateLayout->addWidget(serialBaudRateLabel);
    serialBaudRateLayout->addWidget(serialBaudRateComboBox);
    serialBaudRateComboBox->addItem("9600", "9600");
    serialBaudRateComboBox->addItem("19200", "19200");
    serialBaudRateComboBox->addItem("38400", "38400");
    serialBaudRateComboBox->addItem("115200", "115200");
    serialBaudRateComboBox->setCurrentText("115200");
    serialSessionLayout->addLayout(serialBaudRateLayout);

    serialDataBitsLayout = new QHBoxLayout();
    serialDataBitsLabel = new QLabel("Data Bits:");
    serialDataBitsComboBox = new QComboBox();
    serialDataBitsLayout->addWidget(serialDataBitsLabel);
    serialDataBitsLayout->addWidget(serialDataBitsComboBox);
    serialDataBitsComboBox->addItem("5", "5");
    serialDataBitsComboBox->addItem("6", "6");
    serialDataBitsComboBox->addItem("7", "7");
    serialDataBitsComboBox->addItem("8", "8");
    serialDataBitsComboBox->setCurrentText("8");
    serialSessionLayout->addLayout(serialDataBitsLayout);

    serialStopBitsLayout = new QHBoxLayout();
    serialStopBitsLabel = new QLabel("Stop Bits:");
    serialStopBitsComboBox = new QComboBox();
    serialStopBitsLayout->addWidget(serialStopBitsLabel);
    serialStopBitsLayout->addWidget(serialStopBitsComboBox);
    serialStopBitsComboBox->addItem("1", "1");
    serialStopBitsComboBox->addItem("2", "2");
    serialStopBitsComboBox->setCurrentText("1");
    serialSessionLayout->addLayout(serialStopBitsLayout);

    serialParityLayout = new QHBoxLayout();
    serialParityLabel = new QLabel("Parity:");
    serialParityComboBox = new QComboBox();
    serialParityLayout->addWidget(serialParityLabel);
    serialParityLayout->addWidget(serialParityComboBox);
    serialParityComboBox->addItem(tr("0"), "0");
    serialParityComboBox->addItem(tr("2"), "2");
    serialParityComboBox->addItem(tr("3"), "3");
    serialParityComboBox->addItem(tr("4"), "4");
    serialParityComboBox->addItem(tr("5"), "5");
    serialParityComboBox->setCurrentText("0");
    serialSessionLayout->addLayout(serialParityLayout);

    serialFlowControlLayout = new QHBoxLayout();
    serialFlowControlLabel = new QLabel("Flow Control:");
    serialFlowControlComboBox = new QComboBox();
    serialFlowControlLayout->addWidget(serialFlowControlLabel);
    serialFlowControlLayout->addWidget(serialFlowControlComboBox);
    serialFlowControlComboBox->addItem(tr("0"), "0");
    serialFlowControlComboBox->addItem(tr("1"), "1");
    serialFlowControlComboBox->addItem(tr("2"), "2");
    serialFlowControlComboBox->setCurrentText("0");
    serialSessionLayout->addLayout(serialFlowControlLayout);
}

void NewSessionDialog::newSSHSessionLayout() {
    if (sshSessionLayout != nullptr) {
        return;
    }
    sshSessionLayout = new QVBoxLayout();

    sshHostLayout = new QHBoxLayout();
    sshHostLabel = new QLabel("host:");
    sshHostEdit = new QLineEdit();
    sshHostLayout->addWidget(sshHostLabel);
    sshHostLayout->addWidget(sshHostEdit);
    sshSessionLayout->addLayout(sshHostLayout);

    sshPortLayout = new QHBoxLayout();
    sshPortLabel = new QLabel("port:");
    sshPortEdit = new QLineEdit();
    sshPortLayout->addWidget(sshPortLabel);
    sshPortLayout->addWidget(sshPortEdit);
    sshSessionLayout->addLayout(sshPortLayout);

    sshAuthTypeLayout = new QHBoxLayout();
    sshAuthTypeLabel = new QLabel("auth type:");
    sshAuthTypeEdit = new QComboBox();
    sshAuthTypeLayout->addWidget(sshAuthTypeLabel);
    sshAuthTypeLayout->addWidget(sshAuthTypeEdit);
    sshAuthTypeEdit->addItem("passwd", "passwd");
    sshAuthTypeEdit->addItem("ssh-key", "ssh-key");
    sshAuthTypeEdit->setCurrentText("passwd");
    sshSessionLayout->addLayout(sshAuthTypeLayout);
    sshAuthTypeEdit->setEditable(false);

    sshUserLayout = new QHBoxLayout();
    sshUserLabel = new QLabel("user:");
    sshUserEdit = new QLineEdit();
    sshUserLayout->addWidget(sshUserLabel);
    sshUserLayout->addWidget(sshUserEdit);
    sshSessionLayout->addLayout(sshUserLayout);

    sshPasswdLayout = new QHBoxLayout();
    sshPasswdLabel = new QLabel("passwd:");
    sshPasswdEdit = new QLineEdit();
    sshPasswdLayout->addWidget(sshPasswdLabel);
    sshPasswdLayout->addWidget(sshPasswdEdit);
    sshSessionLayout->addLayout(sshPasswdLayout);

    sshKeyFileLayout = new QHBoxLayout();
    sshKeyFileLabel = new QLabel("key file:");
    sshKeyFileEdit = new QLineEdit();
    sshKeyFileLayout->addWidget(sshKeyFileLabel);
    sshKeyFileLayout->addWidget(sshKeyFileEdit);
    sshSessionLayout->addLayout(sshKeyFileLayout);
    sshKeyFileEdit->setEnabled(false);
}

void NewSessionDialog::newButtonLayout() {
    if (buttonLayout != nullptr) {
        return;
    }
    buttonLayout = new QHBoxLayout();
    cancelButton = new QPushButton("cancel");
    applyButton = new QPushButton("apply");
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(applyButton);

    QObject::connect(applyButton, &QPushButton::clicked, this, &NewSessionDialog::onApplyButtonClicked);
    QObject::connect(cancelButton, &QPushButton::clicked, this, &NewSessionDialog::onCancelButtonClicked);
}

void NewSessionDialog::onApplyButtonClicked() {
    ConfigManager *conf = ConfigManager::getInstance();
    std::string sessionName = sessionNameEidt->text().toStdString();
    if (isEdit) {
        saveSession();
        if (sessionName != oldSessionName) {
            conf->deleteSection(oldSessionName.c_str());
            conf->deleteKey("sessions", oldSessionName.c_str());
        }
    } else {
        std::string type = conf->getString("sessions", sessionName.c_str(), "unknown");
        if (type == "unknown") {
            saveSession();
        } else {
            QMessageBox msgBox;
            msgBox.setText("warning:");
            msgBox.setInformativeText("the session name is already exist, please changed the session name!!! ");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();
        }
    }
}

void NewSessionDialog::onCancelButtonClicked() {
    hide();
}

void NewSessionDialog::saveSession() {
    ConfigManager *conf = ConfigManager::getInstance();
    std::string sessionName = sessionNameEidt->text().toStdString();
    std::string sessionType = sessionTypeComboBox->currentText().toStdString();

    if (sessionType == "local") {
        conf->setCString("sessions", sessionName.c_str(), sessionType.c_str());
    } else if (sessionType == "serial") {
        std::string dev = serialDevComboBox->currentText().toStdString();
        std::string baudRate = serialBaudRateComboBox->currentText().toStdString();
        std::string dataBits = serialDataBitsComboBox->currentText().toStdString();
        std::string stopBits = serialStopBitsComboBox->currentText().toStdString();
        std::string parity = serialParityComboBox->currentText().toStdString();
        std::string flowControl = serialFlowControlComboBox->currentText().toStdString();
        conf->setCString(sessionName.c_str(), "dev", dev.c_str());
        conf->setCString(sessionName.c_str(), "baudRate", baudRate.c_str());
        conf->setCString(sessionName.c_str(), "dataBits", dataBits.c_str());
        conf->setCString(sessionName.c_str(), "stopBits", stopBits.c_str());
        conf->setCString(sessionName.c_str(), "parity", parity.c_str());
        conf->setCString(sessionName.c_str(), "flowControl", flowControl.c_str());
        conf->setCString("sessions", sessionName.c_str(), "serial");

    } else if (sessionType == "ssh") {
        std::string host = sshHostEdit->text().toStdString();
        std::string port = sshPortEdit->text().toStdString();
        std::string authType = sshAuthTypeEdit->currentText().toStdString();
        std::string user = sshUserEdit->text().toStdString();
        std::string passwd = sshPasswdEdit->text().toStdString();
        std::string keyFile = sshKeyFileEdit->text().toStdString();
        conf->setCString(sessionName.c_str(), "host", host.c_str());
        conf->setCString(sessionName.c_str(), "port", port.c_str());
        conf->setCString(sessionName.c_str(), "authType", authType.c_str());
        conf->setCString(sessionName.c_str(), "user", user.c_str());
        conf->setCString(sessionName.c_str(), "passwd", passwd.c_str());
        conf->setCString(sessionName.c_str(), "keyFile", keyFile.c_str());
        conf->setCString("sessions", sessionName.c_str(), "ssh");
    } else {
        qDebug() << "session type error" << endl;
        return;
    }
    hide();
    emit sessionListUpdate();
}
