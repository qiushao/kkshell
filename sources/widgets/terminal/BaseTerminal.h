//
// Created by qiushao on 2020/8/7.
//

#ifndef KKSHELL_BASE_TERMINAL_H
#define KKSHELL_BASE_TERMINAL_H

#include "qtermwidget.h"

class BaseTerminal : public QTermWidget {

Q_OBJECT

public:
    explicit BaseTerminal(QWidget *parent);
    ~BaseTerminal() override;

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    bool isConnect();

    bool isLoggingSession();
    void logSession(const std::string &logPath);
    void disableLogSession();

    bool isLoggingHexSession();
    void logHexSession(const std::string &logPath);
    void disableLogHexSession();

signals:
    void requestDisconnect(BaseTerminal *terminal);

protected:
    void onNewLine(const QString &line);
    void onHexData(const char *data, int len);

protected:
    QFont *font_ = nullptr;
    bool _connect = false;
    bool logging_ = false;
    std::string logPath_;
    FILE *logFp_ = nullptr;
    bool loggingHex_ = false;
    std::string logHexPath_;
    FILE *logHexFp_ = nullptr;
};


#endif //KKSHELL_BASE_TERMINAL_H
