//
// Created by qiushao on 2020/12/4.
//

#ifndef KKSHELL_COMMANDBUTTON_H
#define KKSHELL_COMMANDBUTTON_H

#include <QAction>
#include "setting/Settings.h"

class CommandButton : public QAction {
Q_OBJECT
public:
    explicit CommandButton(const Command &command, QObject *parent = nullptr);
    ~CommandButton() override;

    const Command &command();

signals:
    void clicked(const Command &command);

private:
    void onTrigger();

    Command _command;
};


#endif //KKSHELL_COMMANDBUTTON_H
