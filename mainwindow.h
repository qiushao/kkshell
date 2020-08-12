#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "terminal/base_terminal.h"
#include "settings.h"
#include "session_manager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void onTabChanged(int index);
    void onTabCloseRequested(int index);

    void onActionExit();
    void onActionSetting();
    void onActionNewSerialSession();
    void onActionNewSSHSession();
    void onActionNewLocalShellSession();
    void onActionOpenSession();
    void onActionConnect();
    void onActionDisconnect();
    void onActionCopy();
    void onActionPaste();
    void onActionClear();
    void onActionFind();

private:
    void actionInit();
    void buttonBarInit();

    BaseTerminal* createLocalShellSession(std::string session);
    BaseTerminal* createSerialSession(std::string session);
    BaseTerminal* createSSHSession(std::string session);

private slots:
    void receiveCommand(const QString &command);
    void onOpenSession(std::string session);

private:
    Ui::MainWindow *ui = nullptr;
    Settings *settingDialog = nullptr;
    SessionManager *sessionManager = nullptr;
    QTabWidget *tabWidget = nullptr;
    BaseTerminal *currentTab = nullptr;
};
#endif // MAINWINDOW_H
