//
// Created by qiushao on 2020/12/5.
//

#include "SSHSessionEditor.h"
#include "setting/SettingManager.h"

SSHSessionEditor::SSHSessionEditor(QWidget *parent, const QString &categoryName) : QDialog(parent) {
    _categoryName = categoryName;

    initWidgets();
    _categoryEdit->setText(_categoryName);
    _categoryEdit->setEnabled(false);
    _portEdit->setText("22");
}

SSHSessionEditor::SSHSessionEditor(QWidget *parent, const QString &categoryName, const Session &session) : QDialog(
        parent) {
    _categoryName = categoryName;
    _oldSession = session;
    _isEdit = true;

    initWidgets();
    _categoryEdit->setText(_categoryName);
    _categoryEdit->setEnabled(false);
    _sessionNameEdit->setText(_oldSession.sessionName.c_str());
    _hostEdit->setText(session.host.c_str());
    _portEdit->setText(QString::number(session.port));
    _userNameEdit->setText(session.userName.c_str());
    if (session.authType == SSH_AUTH_TYPE::SSH_KEY) {
        _authTypeEdit->setCurrentText("SSH Key");
    }
    _passwdEdit->setText(session.passwd.c_str());
    _keyFileEdit->setText(session.keyFile.c_str());
}

SSHSessionEditor::~SSHSessionEditor() {

}

void SSHSessionEditor::initWidgets() {
    _mainLayout = new QVBoxLayout(this);
    setLayout(_mainLayout);

    _formLayout = new QFormLayout(this);
    _mainLayout->addLayout(_formLayout);

    _categoryEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Category："), _categoryEdit);
    _sessionNameEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Session Name:"), _sessionNameEdit);

    _hostEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Host:"), _hostEdit);
    _portEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Port:"), _portEdit);
    _userNameEdit = new QLineEdit(this);
    _formLayout->addRow(tr("User Name:"), _userNameEdit);
    _authTypeEdit = new QComboBox(this);
    _authTypeEdit->addItem("Passwd", QVariant::fromValue(SSH_AUTH_TYPE::PASSWD));
    _authTypeEdit->addItem("SSH Key", QVariant::fromValue(SSH_AUTH_TYPE::SSH_KEY));
    _formLayout->addRow(tr("Auth Type:"), _authTypeEdit);
    _passwdEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Passwd"), _passwdEdit);
    _keyFileEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Private Key:"), _keyFileEdit);

    _buttonLayout = new QHBoxLayout(this);
    _mainLayout->addLayout(_buttonLayout);

    _okButton = new QPushButton(tr("OK"));
    _cancelButton = new QPushButton(tr("Cancel"));
    _buttonLayout->addWidget(_cancelButton);
    _buttonLayout->addWidget(_okButton);

    QObject::connect(_okButton, &QPushButton::clicked, this, &SSHSessionEditor::onOK);
    QObject::connect(_cancelButton, &QPushButton::clicked, this, &SSHSessionEditor::onCancel);
}

void SSHSessionEditor::onOK() {
    SettingManager *settingManager = SettingManager::getInstance();
    Session session;
    session.sessionType = SESSION_TYPE::SSH2;
    session.sessionName = _sessionNameEdit->text().toStdString();
    session.host = _hostEdit->text().toStdString();
    session.port = _portEdit->text().toInt();
    session.userName = _userNameEdit->text().toStdString();
    auto var = _authTypeEdit->currentData(Qt::UserRole);
    session.authType = var.value<SSH_AUTH_TYPE>();
    session.passwd = _passwdEdit->text().toStdString();
    session.keyFile = _keyFileEdit->text().toStdString();

    if (_isEdit) {
        settingManager->updateSession(_categoryName.toStdString(), _oldSession.sessionName, session);
        emit updateSession(_categoryName, _oldSession, session);
    } else {
        settingManager->addSession(_categoryName.toStdString(), session);
        emit newSession(_categoryName, session);
    }
    close();
}

void SSHSessionEditor::onCancel() {
    close();
}
