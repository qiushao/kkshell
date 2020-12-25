//
// Created by qiushao on 2020/12/12.
//

#ifndef KKSHELL_COMMANDTOOLBAR_H
#define KKSHELL_COMMANDTOOLBAR_H

#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QComboBox>
#include "widgets/command/CommandButton.h"

class CommandToolBar : public QToolBar {
Q_OBJECT
public:
    explicit CommandToolBar(QWidget *parent = nullptr);
    ~CommandToolBar() override;

    void onPopMenu(const QPoint& point);

signals:
    void sendCommand(const Command &command);

private:
    QString _currentCategory;
    QComboBox *_commandCategorySelect;
    CommandButton *_selectedCommand = nullptr;

    void initWidget();
    void initMenu();
    void selectCategory(const QString &categoryName);
    void onCommandButtonClicked(const Command &command);
    void onNewCommandCategory(const CommandCategory &category);
    void onUpdateCommandCategory(const QString &oldCategoryName, const QString &newCategoryName);

    void onNewCommandAction();
    void onEditCommandAction();
    void onRemoveCommandAction();

    void onNewCommandCategoryAction();
    void onEditCurrentCategoryAction();
    void onRemoveCurrentCategoryAction();

    QMenu *_commandMenu;
    QAction *_newCommandAction;
    QAction *_editCommandAction;
    QAction *_removeCommandAction;

    QMenu *_commandToolBarMenu;
    QAction *_newCommandCategoryAction;
    QAction *_removeCurrentCategoryAction;
    QAction *_editCurrentCategoryAction;
};


#endif //KKSHELL_COMMANDTOOLBAR_H
