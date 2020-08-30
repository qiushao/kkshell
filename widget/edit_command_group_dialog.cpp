//
// Created by min on 2020/8/17.
//

#include "edit_command_group_dialog.h"
#include "common/config/config_manager.h"

EditCommandGroupDialog::EditCommandGroupDialog(QWidget *parent) : QDialog(parent) {
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

    QObject::connect(applyButton, &QPushButton::clicked, this, &EditCommandGroupDialog::onApply);
    QObject::connect(cancelButton, &QPushButton::clicked, this, &EditCommandGroupDialog::onCancel);
}

EditCommandGroupDialog::~EditCommandGroupDialog() {

}

void EditCommandGroupDialog::onApply() {
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
            emit commandGroupChanged(groupName.c_str());
        }
    } else {
        conf->setCString("command-bars", groupName.c_str(), "show");
        emit commandGroupAdd(groupName.c_str());
    }
    hide();
}

void EditCommandGroupDialog::onCancel() {
    hide();
}

void EditCommandGroupDialog::setField(const std::string &groupName) {
    isEdit = true;
    oldGroupName = groupName;
    titleEdit->setText(groupName.c_str());
}

void EditCommandGroupDialog::clearField() {
    isEdit = false;
    oldGroupName = "";
    titleEdit->setText("");
}
