//
// Created by min on 2020/8/17.
//

#include "edit_command_button_dialog.h"
#include "common/config/config_manager.h"

EditCommandButtonDialog::EditCommandButtonDialog(QWidget *parent)
        : QDialog(parent) {
    mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    setModal(true);

    titleLayout = new QHBoxLayout();
    titleLabel = new QLabel(tr("title:"));
    titleEdit = new QLineEdit();
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(titleEdit);
    mainLayout->addLayout(titleLayout);

    commandLayout = new QHBoxLayout();
    commandLabel = new QLabel(tr("command:"));
    commandEdit = new QLineEdit();
    commandLayout->addWidget(commandLabel);
    commandLayout->addWidget(commandEdit);
    mainLayout->addLayout(commandLayout);

    actionLayout = new QHBoxLayout();
    applyButton = new QPushButton(tr("apply"));
    cancelButton = new QPushButton(tr("cancel"));
    actionLayout->addWidget(cancelButton);
    actionLayout->addWidget(applyButton);
    mainLayout->addLayout(actionLayout);

    QObject::connect(applyButton, &QPushButton::clicked, this, &EditCommandButtonDialog::onApply);
    QObject::connect(cancelButton, &QPushButton::clicked, this, &EditCommandButtonDialog::onCancel);
}

EditCommandButtonDialog::~EditCommandButtonDialog() {

}

void EditCommandButtonDialog::onApply() {
    ConfigManager *conf = ConfigManager::getInstance();
    std::string commandName = titleEdit->text().toStdString();
    std::string command = commandEdit->text().toStdString();
    conf->setCString(groupName_.c_str(), commandName.c_str(), command.c_str());
    if (isEdit && commandName != oldCommandName) {
        conf->deleteKey(groupName_.c_str(), oldCommandName.c_str());
    }
    hide();
    emit commandButtonChanged(groupName_.c_str());
}

void EditCommandButtonDialog::onCancel() {
    hide();
}

void EditCommandButtonDialog::clearField() {
    isEdit = false;
    groupName_ = "";
    oldCommandName = "";
    titleEdit->setText("");
    commandEdit->setText("");
}

void EditCommandButtonDialog::setField(const std::string &groupName,
                                       const std::string &commandName,
                                       const std::string &command) {
    isEdit = true;
    groupName_ = groupName;
    oldCommandName = commandName;
    titleEdit->setText(commandName.c_str());
    commandEdit->setText(command.c_str());
}
