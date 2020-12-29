//
// Created by qiushao on 2020/12/12.
//

#include "CommandButtonEditor.h"
#include "setting/SettingManager.h"

CommandButtonEditor::CommandButtonEditor(QWidget *parent, const QString &categoryName) : QDialog(parent) {
    _categoryName = categoryName;
    initWidgets();
    _categoryEdit->setText(_categoryName);
    _categoryEdit->setEnabled(false);
}

CommandButtonEditor::CommandButtonEditor(QWidget *parent, const QString &categoryName, const Command &command)
        : QDialog(parent) {
    _categoryName = categoryName;
    _oldCommand = command;
    _isEdit = true;

    initWidgets();
    _categoryEdit->setText(_categoryName);
    _categoryEdit->setEnabled(false);
    _commandNameEdit->setText(command.commandName.c_str());
    _commandStringEdit->setText(command.commandString.c_str());
}

CommandButtonEditor::~CommandButtonEditor() {

}

void CommandButtonEditor::initWidgets() {
    _mainLayout = new QVBoxLayout(this);
    setLayout(_mainLayout);

    _formLayout = new QFormLayout(this);
    _mainLayout->addLayout(_formLayout);

    _categoryEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Category："), _categoryEdit);
    _commandNameEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Command Name:"), _commandNameEdit);
    _commandStringEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Command String:"), _commandStringEdit);

    _buttonLayout = new QHBoxLayout(this);
    _okButton = new QPushButton(tr("OK"));
    _cancelButton = new QPushButton(tr("Cancel"));
    _buttonLayout->addWidget(_cancelButton);
    _buttonLayout->addWidget(_okButton);
    _mainLayout->addLayout(_buttonLayout);

    QObject::connect(_okButton, &QPushButton::clicked, this, &CommandButtonEditor::onOK);
    QObject::connect(_cancelButton, &QPushButton::clicked, this, &CommandButtonEditor::onCancel);
}

void CommandButtonEditor::onOK() {
    SettingManager *settingManager = SettingManager::getInstance();
    Command command;
    command.commandName = _commandNameEdit->text().toStdString();
    command.commandString = _commandStringEdit->text().toStdString();

    if (_isEdit) {
        settingManager->updateCommand(_categoryName.toStdString(), _oldCommand.commandName, command);
    } else {
        settingManager->addCommand(_categoryName.toStdString(), command);
    }

    emit reloadCommandCategory(_categoryName);
    close();
}

void CommandButtonEditor::onCancel() {
    close();
}
