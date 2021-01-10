//
// Created by qiushao on 2020/11/1.
//

#ifndef KKSHELL_MAINWINDOW_H
#define KKSHELL_MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QIcon>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QComboBox>
#include <QTabWidget>
#include <QDockWidget>
#include <QTextEdit>
#include "widgets/session/SessionListWidget.h"
#include "widgets/terminal/BaseTerminal.h"
#include "widgets/command/CommandToolBar.h"
#include "widgets/tabwidget/SessionTabWidget.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void initIcons();
    void initActions();
    void initMenu();
    void initToolbar();
    void initSessionList();
    void initTableWidget();
    void initCommandWindow();
    void initCommandBar();

    void onSettingsAction();
    void onToggleSaveLogAction();
    void onToggleSaveHexLogAction();
    void onConnectAction();
    void onDisconnectAction();
    void onExitAction();

    void onCopySelectedAction();
    void onCopyAllAction();
    void onPasteAction();
    void onFindAction();
    void onClearScreenAction();

    void onToggleToolbarAction();
    void onToggleSessionManagerAction();
    void onToggleCommandWindowAction();
    void onToggleCommandButtonAction();

    void onRequestOpenSession(const Session &session);
    void onRequestDisconnect(BaseTerminal *terminal);
    void onTabChanged(int index);
    void onTabCloseRequested(int index);

    bool eventFilter(QObject *obj, QEvent *e) override;
    void receiveCommand(const Command &command);
    std::string selectLogPath();

private:
    QMenuBar *_mainMenuBar;
    QMenu *_fileMenu;
    QMenu *_editMenu;
    QMenu *_viewMenu;
    QMenu *_helpMenu;

    QToolBar *_toolBar;
    CommandToolBar *_commandBar;

    QIcon *_settingsIcon;
    QIcon *_connectIcon;
    QIcon *_disconnectIcon;
    QIcon *_connectStateIcon;
    QIcon *_disconnectStateIcon;
    QIcon *_exitIcon;

    QIcon *_copySelectedIcon;
    QIcon *_copyAllIcon;
    QIcon *_pasteIcon;
    QIcon *_findIcon;
    QIcon *_clearScreenIcon;

    QIcon *_toggleOnIcon;
    QIcon *_toggleOffIcon;

    QAction *_settingsAction;
    QAction *_toggleSaveLogAction;
    QAction *_toggleSaveHexLogAction;
    QAction *_connectAction;
    QAction *_disConnectAction;
    QAction *_exitAction;

    QAction *_copySelectedAction;
    QAction *_copyAllAction;
    QAction *_pasteAction;
    QAction *_findAction;
    QAction *_clearScreenAction;

    QAction *_toggleToolbarAction;
    QAction *_toggleSessionManagerAction;
    QAction *_toggleCommandWindowAction;
    QAction *_toggleCommandButtonAction;

    //terminal list
    SessionTabWidget *_tabWidget = nullptr;
    BaseTerminal *_currentTab = nullptr;

    //session manager
    QDockWidget *_sessionDock = nullptr;
    SessionListWidget *_sessionListWidget = nullptr;

    //command window
    QDockWidget *_commandWindowDock = nullptr;
    QTextEdit *_commandWindowWidget = nullptr;
};


#endif //KKSHELL_MAINWINDOW_H
