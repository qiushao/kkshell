//
// Created by mint on 2020/8/4.
//

#ifndef KKSHELL_COMMAND_BUTTON_H
#define KKSHELL_COMMAND_BUTTON_H

#include <QPushButton>

class CommandButton : public QPushButton{

Q_OBJECT

public:
    explicit CommandButton(const QString &title, const QString &command, QWidget *parent = nullptr);
    ~CommandButton() override;

    void contextMenuEvent(QContextMenuEvent *) override;

    signals:
    void sendCommand(const QString &command);
private:
    void onClicked();

private:
    QString title_;
    QString command_;
};


#endif //KKSHELL_COMMAND_BUTTON_H
