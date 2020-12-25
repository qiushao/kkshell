//
// Created by qiushao on 2020/8/7.
//

#ifndef KKSHELL_LOCAL_TERMINAL_H
#define KKSHELL_LOCAL_TERMINAL_H

#include "BaseTerminal.h"

class LocalTerminal : public BaseTerminal {
public:
    explicit LocalTerminal(QWidget *parent);
    ~LocalTerminal() override;
public:
    void connect() override;
    void disconnect() override;

};


#endif //KKSHELL_LOCAL_TERMINAL_H
