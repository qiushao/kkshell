//
// Created by min on 2020/8/17.
//

#ifndef KKSHELL_EDIT_COMMAND_GROUP_DIALOG_H
#define KKSHELL_EDIT_COMMAND_GROUP_DIALOG_H

#include "new_command_group_dialog.h"

class EditCommandGroupDialog : public NewCommandGroupDialog{
public:
    EditCommandGroupDialog(std::string groupName, QWidget *parent);
    ~EditCommandGroupDialog() override;

};


#endif //KKSHELL_EDIT_COMMAND_GROUP_DIALOG_H
