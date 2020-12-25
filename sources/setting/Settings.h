//
// Created by qiushao on 2020/10/27.
//

#ifndef KKSHELL_SETTINGS_H
#define KKSHELL_SETTINGS_H

#include <string>
#include <vector>
#include <QObject>

enum SESSION_TYPE {
    LOCAL_SHELL = 0,
    SSH2 = 1,
    SERIAL = 2,
    TELNET = 3
};

enum SSH_AUTH_TYPE {
    PASSWD = 0,
    SSH_KEY = 1
};

Q_DECLARE_METATYPE(SSH_AUTH_TYPE)

struct Session {
    std::string sessionName;
    SESSION_TYPE sessionType;

    //for ssh session or telnet
    std::string host;
    int port;
    std::string userName;
    SSH_AUTH_TYPE authType;
    std::string passwd;
    std::string keyFile; //public key

    //for serial session
    std::string dev;
    int baudRate;
    int dataBits;
    int stopBits;
    int parity;
    int flowControl;
};

Q_DECLARE_METATYPE(Session)

struct SessionCategory {
    std::string categoryName;
    std::vector<Session> sessionList;
};

Q_DECLARE_METATYPE(SessionCategory)

struct Command {
    std::string commandName;
    std::string commandString;
};

struct CommandCategory {
    std::string categoryName;
    std::vector<Command> commandList;
};



struct Settings {
    std::string appVersion;
    std::string fontFamily;
    int fontSize;
    std::string colorScheme;
    std::vector<SessionCategory> sessionCategoryList;
    std::vector<CommandCategory> commandCategoryList;
};

#endif //KKSHELL_SETTINGS_H
