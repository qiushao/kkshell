//
// Created by mint on 2020/8/11.
//

#ifndef KKSHELL_EDIT_SESSION_DIALOG_H
#define KKSHELL_EDIT_SESSION_DIALOG_H

#include "new_session_dialog.h"

class EditSessionDialog : public NewSessionDialog {
public:
    EditSessionDialog(const std::string &sessionName, const std::string &sessionType, QWidget *parent);
    ~EditSessionDialog() override;

private:
    void editSerialSession(const std::string &sessionName);
    void editSSHSession(const std::string &sessionName);
};


#endif //KKSHELL_EDIT_SESSION_DIALOG_H
