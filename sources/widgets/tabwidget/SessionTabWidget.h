//
// Created by qiushao on 2021/1/6.
//

#ifndef KKSHELL_SESSIONTABWIDGET_H
#define KKSHELL_SESSIONTABWIDGET_H

#include <QTabWidget>
#include <QMenu>
#include <QAction>

class SessionTabWidget : public QTabWidget {
Q_OBJECT

public:
    explicit SessionTabWidget(QWidget *parent = nullptr);
    ~SessionTabWidget() override;

private:
    void onPopMenu(const QPoint& point);
    void onRenameTableAction();
    void onCloseTableAction();
    void onCloseOtherTableAction();
    void onCloseAllTableAction();

    void updateTableTitle(const QString &newTitle);

    int _tabIndex = -1;
    QMenu *_menu = nullptr;
    QAction *_renameTableAction = nullptr;
    QAction *_closeTableAction = nullptr;
    QAction *_closeOtherTableAction = nullptr;
    QAction *_closeAllTableAction = nullptr;
};


#endif //KKSHELL_SESSIONTABWIDGET_H
