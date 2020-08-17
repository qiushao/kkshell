//
// Created by min on 2020/8/17.
//

#include "new_command_group_dialog.h"
#include "common/config/config_manager.h"

NewCommandGroupDialog::NewCommandGroupDialog(QWidget *parent) : QDialog(parent) {
    mainLayout = new QVBoxLayout();
    setLayout(mainLayout);

    titleLayout = new QHBoxLayout();
    titleLabel = new QLabel(tr("command group name:"));
    titleEdit = new QLineEdit();
    titleLayout->addWidget(titleLabel);
    titleLayout->addWidget(titleEdit);
    mainLayout->addLayout(titleLayout);

    actionLayout = new QHBoxLayout();
    applyButton = new QPushButton(tr("apply"));
    cancelButton = new QPushButton(tr("cancel"));
    actionLayout->addWidget(cancelButton);
    actionLayout->addWidget(applyButton);
    mainLayout->addLayout(actionLayout);

    QObject::connect(applyButton, &QPushButton::clicked, this, &NewCommandGroupDialog::onApply);
    QObject::connect(cancelButton, &QPushButton::clicked, this, &NewCommandGroupDialog::onCancel);
}

NewCommandGroupDialog::~NewCommandGroupDialog() {

}

void NewCommandGroupDialog::onApply() {
    ConfigManager *conf = ConfigManager::getInstance();
    std::string groupName = titleEdit->text().toStdString();
    if (isEdit) {
        if (groupName != oldGroupName) {
            conf->setCString("command-bars", groupName.c_str(), "show");
            conf->deleteKey("command-bars", oldGroupName.c_str());
            std::vector<std::string> keys = conf->getSectionKeys(oldGroupName.c_str());
            for (const std::string &key : keys) {
                std::string value = conf->getString(oldGroupName.c_str(), key.c_str());
                conf->setCString(groupName.c_str(), key.c_str(), value.c_str());
            }
            conf->deleteSection(oldGroupName.c_str());
        }
    } else {
        conf->setCString("command-bars", groupName.c_str(), "show");
    }

    hide();
}

void NewCommandGroupDialog::onCancel() {
    hide();
}
