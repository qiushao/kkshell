//
// Created by min on 2020/8/17.
//

#ifndef KKSHELL_EDIT_COMMAND_BUTTON_DIALOG_H
#define KKSHELL_EDIT_COMMAND_BUTTON_DIALOG_H

#include "new_command_button_dialog.h"

class EditCommandButtonDialog : public NewCommandButtonDialog {
public:
    EditCommandButtonDialog(QWidget *parent);
    ~EditCommandButtonDialog() override;

    void reset(const std::string &groupName, const std::string &commandName, const std::string &command);
};


#endif //KKSHELL_EDIT_COMMAND_BUTTON_DIALOG_H
