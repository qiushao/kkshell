//
// Created by min on 2020/8/17.
//

#ifndef KKSHELL_EDIT_COMMAND_BUTTON_DIALOG_H
#define KKSHELL_EDIT_COMMAND_BUTTON_DIALOG_H

#include "new_command_button_dialog.h"

class EditCommandButtonDialog : public NewCommandButtonDialog {
public:
    EditCommandButtonDialog(std::string groupName, std::string commandName, std::string command, QWidget *parent);
    ~EditCommandButtonDialog() override;

};


#endif //KKSHELL_EDIT_COMMAND_BUTTON_DIALOG_H
