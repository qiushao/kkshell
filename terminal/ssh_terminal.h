//
// Created by min on 2020/8/7.
//

#ifndef KKSHELL_SSH_TERMINAL_H
#define KKSHELL_SSH_TERMINAL_H

#include "base_terminal.h"
#include <thread>
#include <libssh2.h>

struct SSHSettings {
    std::string host;
    int port;
    std::string authType;
    std::string user;
    std::string passwd;
    std::string keyFile;
};

class SSHTerminal : public BaseTerminal {
public:
    explicit SSHTerminal(const SSHSettings &settings, QWidget *parent);
    ~SSHTerminal() override;
    void connect() override;
    void disconnect() override;

private:
    QString createShellFile();
    bool isInRemoteServer();
    SSHSettings settings_;
};


#endif //KKSHELL_SSH_TERMINAL_H
