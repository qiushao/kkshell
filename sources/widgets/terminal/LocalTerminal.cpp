//
// Created by qiushao on 2020/8/7.
//

#include "LocalTerminal.h"

LocalTerminal::LocalTerminal(QWidget *parent) : BaseTerminal(parent) {

}

LocalTerminal::~LocalTerminal() {

}

void LocalTerminal::connect() {
    startShellProgram();
    _connect = true;
}

void LocalTerminal::disconnect() {
    _connect = false;
}


