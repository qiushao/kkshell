//
// Created by min on 2020/8/17.
//

#include "edit_command_button_dialog.h"

EditCommandButtonDialog::EditCommandButtonDialog(QWidget *parent)
: NewCommandButtonDialog(parent){
    isEdit = true;
}

EditCommandButtonDialog::~EditCommandButtonDialog() {

}

void EditCommandButtonDialog::reset(const std::string &groupName, const std::string &commandName, const std::string &command) {
    groupName_ = groupName;
    oldCommandName = commandName;
    titleEdit->setText(oldCommandName.c_str());
    commandEdit->setText(command.c_str());
}
