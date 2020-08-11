//
// Created by min on 2020/8/7.
//

#ifndef KKSHELL_BASE_TERMINAL_H
#define KKSHELL_BASE_TERMINAL_H

#include <qtermwidget5/qtermwidget.h>

class BaseTerminal : public QTermWidget {
public:
    explicit BaseTerminal(QWidget *parent);
    ~BaseTerminal() override;

    virtual void connect() = 0;
    virtual void disconnect() = 0;
    void clear();
    bool isConnect();

protected:
    bool connect_;
};


#endif //KKSHELL_BASE_TERMINAL_H
