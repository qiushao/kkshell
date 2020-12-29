//
// Created by qiushao on 2020/12/5.
//

#include <QSerialPortInfo>
#include <QSerialPort>
#include "SerialSessionEditor.h"
#include "setting/SettingManager.h"

SerialSessionEditor::SerialSessionEditor(QWidget *parent, const QString &categoryName) : QDialog(parent) {
    _categoryName = categoryName;

    initWidgets();
    _categoryEdit->setText(_categoryName);
    _categoryEdit->setEnabled(false);
    _baudRateEdit->setText("115200");
    _dataBitsEdit->setText("8");
    _stopBitsEdit->setText("1");
    _parityEdit->setText("0");
    _flowControlEdit->setText("0");
}

SerialSessionEditor::SerialSessionEditor(QWidget *parent, const QString &categoryName, const Session &session)
        : QDialog(parent) {
    _categoryName = categoryName;
    _oldSession = session;
    _isEdit = true;

    initWidgets();
    _categoryEdit->setText(_categoryName);
    _categoryEdit->setEnabled(false);
    _sessionNameEdit->setText(_oldSession.sessionName.c_str());

    _devEdit->setCurrentText(session.dev.c_str());
    _baudRateEdit->setText(QString::number(session.baudRate));
    _dataBitsEdit->setText(QString::number(session.dataBits));
    _stopBitsEdit->setText(QString::number(session.stopBits));
    _parityEdit->setText(QString::number(session.parity));
    _flowControlEdit->setText(QString::number(session.flowControl));
}

SerialSessionEditor::~SerialSessionEditor() {

}

void SerialSessionEditor::initWidgets() {
    _mainLayout = new QVBoxLayout(this);
    setLayout(_mainLayout);

    _formLayout = new QFormLayout(this);
    _mainLayout->addLayout(_formLayout);

    _categoryEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Category："), _categoryEdit);
    _sessionNameEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Session Name:"), _sessionNameEdit);

    _devEdit = new QComboBox(this);
    _formLayout->addRow(tr("Serial Device:"), _devEdit);
    const auto devices = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &dev : devices) {
        _devEdit->addItem(dev.portName(), dev.portName());
    }

    _baudRateEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Baud Rate:"), _baudRateEdit);
    _dataBitsEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Data Bits:"), _dataBitsEdit);
    _stopBitsEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Stop Bits:"), _stopBitsEdit);
    _parityEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Parity:"), _parityEdit);
    _flowControlEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Flow Control:"), _flowControlEdit);

    _buttonLayout = new QHBoxLayout(this);
    _mainLayout->addLayout(_buttonLayout);

    _okButton = new QPushButton(tr("OK"));
    _cancelButton = new QPushButton(tr("Cancel"));
    _buttonLayout->addWidget(_cancelButton);
    _buttonLayout->addWidget(_okButton);

    QObject::connect(_okButton, &QPushButton::clicked, this, &SerialSessionEditor::onOK);
    QObject::connect(_cancelButton, &QPushButton::clicked, this, &SerialSessionEditor::onCancel);
}

void SerialSessionEditor::onOK() {
    SettingManager *settingManager = SettingManager::getInstance();
    Session session;
    session.sessionType = SESSION_TYPE::SERIAL;
    session.sessionName = _sessionNameEdit->text().toStdString();

    session.dev = _devEdit->currentText().toStdString();
    session.baudRate = _baudRateEdit->text().toInt();
    session.dataBits = _dataBitsEdit->text().toInt();
    session.stopBits = _stopBitsEdit->text().toInt();
    session.parity = _parityEdit->text().toInt();
    session.flowControl = _flowControlEdit->text().toInt();

    if (_isEdit) {
        settingManager->updateSession(_categoryName.toStdString(), _oldSession.sessionName, session);
        emit updateSession(_categoryName, _oldSession, session);
    } else {
        settingManager->addSession(_categoryName.toStdString(), session);
        emit newSession(_categoryName, session);
    }
    close();
}

void SerialSessionEditor::onCancel() {
    close();
}
