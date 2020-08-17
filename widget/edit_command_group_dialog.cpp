//
// Created by min on 2020/8/17.
//

#include "edit_command_group_dialog.h"

EditCommandGroupDialog::EditCommandGroupDialog(std::string groupName, QWidget *parent) : NewCommandGroupDialog(parent) {
    isEdit = true;
    oldGroupName = groupName;
    titleEdit->setText(groupName.c_str());
}

EditCommandGroupDialog::~EditCommandGroupDialog() {

}
