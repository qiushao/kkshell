//
// Created by mint on 2020/8/4.
//

#include "command_button.h"
#include <QMenu>
#include <QDebug>

CommandButton::CommandButton(const QString &title, const QString &command, QWidget *parent) : QPushButton(title, parent) {
    title_ = title;
    command_ = command;
    connect(this, &QPushButton::clicked, this, &CommandButton::onClicked);
}

CommandButton::~CommandButton() {
    delete commandButtonMenu_;
    delete actionEditCommand_;
    delete actionDeleteCommand_;
}

void CommandButton::onClicked() {
    emit sendCommand(command_);
}

void CommandButton::contextMenuEvent(QContextMenuEvent *) {
    if (commandButtonMenu_ == nullptr) {
        initMenu();
    }
    commandButtonMenu_->exec(QCursor::pos());
}

void CommandButton::initMenu() {
    commandButtonMenu_ = new QMenu(this);
    actionEditCommand_ = new QAction(tr("edit command"));
    actionDeleteCommand_ = new QAction(tr("delete command"));
    commandButtonMenu_->addAction(actionEditCommand_);
    commandButtonMenu_->addAction(actionDeleteCommand_);
    QObject::connect(actionEditCommand_, &QAction::triggered, this, &CommandButton::onActionEditCommand);
    QObject::connect(actionDeleteCommand_, &QAction::triggered, this, &CommandButton::onActionDeleteCommand);
}

void CommandButton::onActionEditCommand() {
    qDebug() << "onActionEditCommand" << endl;
    emit requestEditCommandButton(title_, command_);
}

void CommandButton::onActionDeleteCommand() {
    qDebug() << "onActionDeleteCommand" << endl;
    emit requestDeleteCommandButton(title_);
}
