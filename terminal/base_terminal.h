//
// Created by min on 2020/8/7.
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
    void clear();
    bool isConnect();

signals:
    void requestDisconnect(BaseTerminal *terminal);

protected:
    QFont *font_;
    bool connect_;
};


#endif //KKSHELL_BASE_TERMINAL_H
