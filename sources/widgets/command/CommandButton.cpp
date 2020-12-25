//
// Created by qiushao on 2020/12/4.
//

#include "CommandButton.h"

CommandButton::CommandButton(const Command &command, QObject *parent) : QAction(command.commandName.c_str(), parent) {
    _command = command;
    setToolTip(command.commandString.c_str());
    connect(this, &QAction::triggered, this, &CommandButton::onTrigger);
}

CommandButton::~CommandButton() {
    disconnect(this, &QAction::triggered, this, &CommandButton::onTrigger);
}

void CommandButton::onTrigger() {
    emit clicked(_command);
}

const Command &CommandButton::command() {
    return _command;
}
