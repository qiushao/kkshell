//
// Created by min on 2020/8/17.
//

#include "edit_command_button_dialog.h"

EditCommandButtonDialog::EditCommandButtonDialog(std::string groupName, std::string commandName, std::string command, QWidget *parent)
: NewCommandButtonDialog(groupName, parent){
    isEdit = true;
    oldCommandName = commandName;
    titleEdit->setText(oldCommandName.c_str());
    commandEdit->setText(command.c_str());
}

EditCommandButtonDialog::~EditCommandButtonDialog() {

}
