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
    void requestEditCommandButton(const QString &commandName, const QString &command);
    void requestDeleteCommandButton(const QString &commandName);
private:
    void initMenu();
    void onClicked();

    void onActionEditCommand();
    void onActionDeleteCommand();

private:
    QString title_;
    QString command_;

    QMenu *commandButtonMenu_ = nullptr;
    QAction *actionEditCommand_ = nullptr;
    QAction *actionDeleteCommand_ = nullptr;
};


#endif //KKSHELL_COMMAND_BUTTON_H
