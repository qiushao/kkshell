//
// Created by qiushao on 2020/12/5.
//

#include "LocalSessionEditor.h"
#include "setting/SettingManager.h"

LocalSessionEditor::LocalSessionEditor(QWidget *parent, const QString &categoryName) : QDialog(parent) {
    _categoryName = categoryName;

    initWidgets();
    _categoryEdit->setText(_categoryName);
    _categoryEdit->setEnabled(false);
}

LocalSessionEditor::LocalSessionEditor(QWidget *parent, const QString &categoryName, const Session &session) : QDialog(
        parent) {
    _categoryName = categoryName;
    _oldSession = session;
    _isEdit = true;

    initWidgets();
    _categoryEdit->setText(_categoryName);
    _categoryEdit->setEnabled(false);
    _sessionNameEdit->setText(_oldSession.sessionName.c_str());
}

LocalSessionEditor::~LocalSessionEditor() {

}

void LocalSessionEditor::initWidgets() {
    _mainLayout = new QVBoxLayout(this);
    setLayout(_mainLayout);

    _formLayout = new QFormLayout(this);
    _mainLayout->addLayout(_formLayout);

    _categoryEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Category："), _categoryEdit);
    _sessionNameEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Session Name:"), _sessionNameEdit);

    _buttonLayout = new QHBoxLayout(this);
    _okButton = new QPushButton(tr("OK"));
    _cancelButton = new QPushButton(tr("Cancel"));
    _buttonLayout->addWidget(_cancelButton);
    _buttonLayout->addWidget(_okButton);
    _mainLayout->addLayout(_buttonLayout);

    QObject::connect(_okButton, &QPushButton::clicked, this, &LocalSessionEditor::onOK);
    QObject::connect(_cancelButton, &QPushButton::clicked, this, &LocalSessionEditor::onCancel);
}

void LocalSessionEditor::onOK() {
    SettingManager *settingManager = SettingManager::getInstance();
    Session session;
    session.sessionType = SESSION_TYPE::LOCAL_SHELL;
    session.sessionName = _sessionNameEdit->text().toStdString();

    if (_isEdit) {
        settingManager->updateSession(_categoryName.toStdString(), _oldSession.sessionName, session);
        emit updateSession(_categoryName, _oldSession, session);
    } else {
        settingManager->addSession(_categoryName.toStdString(), session);
        emit newSession(_categoryName, session);
    }
    close();
}

void LocalSessionEditor::onCancel() {
    close();
}
