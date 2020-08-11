//
// Created by min on 2020/8/7.
//

#include "local_terminal.h"

LocalTerminal::LocalTerminal(QWidget *parent) : BaseTerminal(parent) {

}

LocalTerminal::~LocalTerminal() {

}

void LocalTerminal::connect() {
    startShellProgram();
    connect_ = true;
}

void LocalTerminal::disconnect() {
    connect_ = false;
}


