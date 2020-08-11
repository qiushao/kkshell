//
// Created by mint on 2020/8/4.
//

#include "command_button.h"
#include <QMenu>

CommandButton::CommandButton(const QString &title, const QString &command, QWidget *parent) : QPushButton(title, parent) {
    title_ = title;
    command_ = command;
    connect(this, &QPushButton::clicked, this, &CommandButton::onClicked);
}

CommandButton::~CommandButton() {

}

void CommandButton::onClicked() {
    emit sendCommand(command_);
}

void CommandButton::contextMenuEvent(QContextMenuEvent *) {
    QCursor cur=this->cursor();
    QMenu *menu=new QMenu(this);
    QAction *editAction = new QAction(tr("编辑"));
    QAction *delAction = new QAction(tr("删除"));
    menu->addAction(editAction);
    menu->addAction(delAction);
    menu->exec(cur.pos());
}
