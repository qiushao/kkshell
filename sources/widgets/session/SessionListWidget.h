//
// Created by qiushao on 2020/11/1.
//

#ifndef KKSHELL_SESSIONLISTWIDGET_H
#define KKSHELL_SESSIONLISTWIDGET_H

#include <QTreeWidget>
#include <QItemDelegate>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include "Settings.h"

class SessionListWidget : public QTreeWidget {
Q_OBJECT
public:
    explicit SessionListWidget(QWidget *parent = nullptr);
    ~SessionListWidget() override ;

    void loadSessionList();

signals:
    void requestOpenSession(const Session &session);

private:
    void initIcon();
    void initAction();
    void initPopMenu();

// slot
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onPopMenu(const QPoint& point);
    void onNewSession(const QString &categoryName, const Session &session);
    void onUpdateSession(const QString &categoryName, const Session &oldSession, const Session &newSession);

    void onNewCategoryAction();
    void onNewSessionCategory(const SessionCategory &category);
    void onRenameCategoryAction();
    void onRenameSessionCategory(const QString &oldCategoryName, const QString &newCategoryName);
    void onRemoveCategoryAction();
    void onMoveUpCategoryAction();
    void onMoveDownCategoryAction();

    void onNewLocalSessionAction();
    void onNewSerialSessionAction();
    void onNewSSHSessionAction();
    void onEditSessionActioin();
    void onRemoveSessionAction();
    void onMoveUpSessionAction();
    void onMoveDownSessionAction();

    QIcon *_categoryIcon;
    QIcon *_sessionIcon;

    QMenu *_emptyAreaMenu;
    QMenu *_categoryMenu;
    QMenu *_sessionMenu;

    QAction *_newCategoryAction;
    QAction *_renameCategoryAction;
    QAction *_removeCategoryAction;
    QAction *_moveUpCategoryAction;
    QAction *_moveDownCategoryAction;

    QAction *_newLocalSessionAction;
    QAction *_newSerialSessionAction;
    QAction *_newSSHSessionAction;
    QAction *_editSessionAction;
    QAction *_removeSessionAction;
    QAction *_moveUpSessionAction;
    QAction *_moveDownSessionAction;
};


#endif //KKSHELL_SESSIONLISTWIDGET_H
