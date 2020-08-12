//
// Created by mint on 2020/8/12.
//

#include "new_session_dialog.h"
#include <QDebug>
#include <QSerialPortInfo>
#include <QSerialPort>
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
    serialBaudRateComboBox->addItem("9600", QSerialPort::Baud9600);
    serialBaudRateComboBox->addItem("19200", QSerialPort::Baud19200);
    serialBaudRateComboBox->addItem("38400", QSerialPort::Baud38400);
    serialBaudRateComboBox->addItem("115200", QSerialPort::Baud115200);
    serialBaudRateComboBox->setCurrentText("115200");
    serialSessionLayout->addLayout(serialBaudRateLayout);

    serialDataBitsLayout = new QHBoxLayout();
    serialDataBitsLabel = new QLabel("Data Bits:");
    serialDataBitsComboBox = new QComboBox();
    serialDataBitsLayout->addWidget(serialDataBitsLabel);
    serialDataBitsLayout->addWidget(serialDataBitsComboBox);
    serialDataBitsComboBox->addItem("5", QSerialPort::Data5);
    serialDataBitsComboBox->addItem("6", QSerialPort::Data6);
    serialDataBitsComboBox->addItem("7", QSerialPort::Data7);
    serialDataBitsComboBox->addItem("8", QSerialPort::Data8);
    serialDataBitsComboBox->setCurrentText("8");
    serialSessionLayout->addLayout(serialDataBitsLayout);

    serialStopBitsLayout = new QHBoxLayout();
    serialStopBitsLabel = new QLabel("Stop Bits:");
    serialStopBitsComboBox = new QComboBox();
    serialStopBitsLayout->addWidget(serialStopBitsLabel);
    serialStopBitsLayout->addWidget(serialStopBitsComboBox);
    serialStopBitsComboBox->addItem("1", QSerialPort::OneStop);
    serialStopBitsComboBox->addItem("2", QSerialPort::TwoStop);
    serialStopBitsComboBox->setCurrentText("1");
    serialSessionLayout->addLayout(serialStopBitsLayout);

    serialParityLayout = new QHBoxLayout();
    serialParityLabel = new QLabel("Parity:");
    serialParityComboBox = new QComboBox();
    serialParityLayout->addWidget(serialParityLabel);
    serialParityLayout->addWidget(serialParityComboBox);
    serialParityComboBox->addItem(tr("None"), QSerialPort::NoParity);
    serialParityComboBox->addItem(tr("Even"), QSerialPort::EvenParity);
    serialParityComboBox->addItem(tr("Odd"), QSerialPort::OddParity);
    serialParityComboBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    serialParityComboBox->addItem(tr("Space"), QSerialPort::SpaceParity);
    serialParityComboBox->setCurrentText("None");
    serialSessionLayout->addLayout(serialParityLayout);

    serialFlowControlLayout = new QHBoxLayout();
    serialFlowControlLabel = new QLabel("Flow Control:");
    serialFlowControlComboBox = new QComboBox();
    serialFlowControlLayout->addWidget(serialFlowControlLabel);
    serialFlowControlLayout->addWidget(serialFlowControlComboBox);
    serialFlowControlComboBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    serialFlowControlComboBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    serialFlowControlComboBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
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
}