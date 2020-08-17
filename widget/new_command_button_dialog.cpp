//
// Created by min on 2020/8/17.
//

#include "new_command_button_dialog.h"
#include "common/config/config_manager.h"

NewCommandButtonDialog::NewCommandButtonDialog(std::string groupName, QWidget *parent)
        : QDialog(parent), groupName_(groupName) {
    mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

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

    QObject::connect(applyButton, &QPushButton::clicked, this, &NewCommandButtonDialog::onApply);
    QObject::connect(cancelButton, &QPushButton::clicked, this, &NewCommandButtonDialog::onCancel);
}

NewCommandButtonDialog::~NewCommandButtonDialog() {

}

void NewCommandButtonDialog::onApply() {
    ConfigManager *conf = ConfigManager::getInstance();
    std::string commandName = titleEdit->text().toStdString();
    std::string command = commandEdit->text().toStdString();
    conf->setCString(groupName_.c_str(), commandName.c_str(), command.c_str());
    if (isEdit && commandName != oldCommandName) {
        conf->deleteKey(groupName_.c_str(), oldCommandName.c_str());
    }
    hide();
}

void NewCommandButtonDialog::onCancel() {
    hide();
}
