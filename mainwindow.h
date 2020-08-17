#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "terminal/base_terminal.h"
#include "settings.h"
#include "session_manager.h"
#include "widget/new_session_dialog.h"

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
    void onActionSelectAll();
    void onActionCopy();
    void onActionPaste();
    void onActionClear();
    void onActionFind();
    void onActionShowToolsBar();
    void onActionShowButtonBar();
    void onActionShowStatusBar();

private:
    void actionInit();
    void buttonBarInit();
    void loadCommandBar(const std::string &commandBar);
    void onCommandBarChanged(const QString &commandBar);

    BaseTerminal* createLocalShellSession(std::string session);
    BaseTerminal* createSerialSession(std::string session);
    BaseTerminal* createSSHSession(std::string session);

private slots:
    void receiveCommand(const QString &command);
    void onOpenSession(std::string session);
    void onRequestDisconnect(BaseTerminal *terminal);

private:
    Ui::MainWindow *ui = nullptr;
    Settings *settingDialog = nullptr;
    SessionManager *sessionManager = nullptr;
    NewSessionDialog *newLocalShellSessionDialog = nullptr;
    NewSessionDialog *newSSHSessionDialog = nullptr;
    NewSessionDialog *newSerialSessionDialog = nullptr;
    QTabWidget *tabWidget = nullptr;
    BaseTerminal *currentTab = nullptr;
    QIcon *connectStateIcon = nullptr;
    QIcon *disconnectStateIcon = nullptr;

    bool showToolsBar = true;
    bool showButtonBar = true;
    bool showStatusBar = true;

    QComboBox *commandBarGroup = nullptr;
    QWidget *commandBarLayoutWidget = nullptr;
    QHBoxLayout *commandButtonLayout = nullptr;
};
#endif // MAINWINDOW_H
