//
// Created by qiushao on 2020/8/7.
//

#ifndef KKSHELL_SSH_TERMINAL_H
#define KKSHELL_SSH_TERMINAL_H

#include "BaseTerminal.h"
#include "setting/Settings.h"

class SSHTerminal : public BaseTerminal {
public:
    explicit SSHTerminal(const Session &session, QWidget *parent);
    ~SSHTerminal() override;
    void connect() override;
    void disconnect() override;

private:
    QString createShellFile();
    bool isInRemoteServer();
    Session _session;
};


#endif //KKSHELL_SSH_TERMINAL_H
