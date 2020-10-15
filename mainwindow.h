#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QTextEdit>
#include "terminal/base_terminal.h"
#include "settings.h"
#include "session_manager.h"
#include "widget/edit_session_dialog.h"
#include "widget/edit_command_group_dialog.h"
#include "widget/edit_command_button_dialog.h"

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

    std::string selectLogPath();
    void onActionLogSession();
    void onActionDisableLogSession();

    bool eventFilter(QObject *obj, QEvent *e) override;

private:
    void actionInit();
    void buttonBarInit();
    void loadCommandBar(const QString &groupName);
    void onCommandGroupChanged(const QString &groupName);
    void onCommandGroupAdd(const QString &groupName);

    BaseTerminal* createLocalShellSession(std::string session);
    BaseTerminal* createSerialSession(std::string session);
    BaseTerminal* createSSHSession(std::string session);

private slots:
    void receiveCommand(const QString &command);
    void onEditCommandButton(const QString &commandName, const QString &command);
    void onDeleteCommandButton(const QString &commandName);

    void onOpenSession(std::string session);
    void onRequestDisconnect(BaseTerminal *terminal);

private:
    Ui::MainWindow *ui = nullptr;
    Settings *settingDialog = nullptr;
    SessionManager *sessionManager = nullptr;
    EditSessionDialog *newLocalShellSessionDialog = nullptr;
    EditSessionDialog *newSSHSessionDialog = nullptr;
    EditSessionDialog *newSerialSessionDialog = nullptr;
    QSplitter *splitter = nullptr;
    QTabWidget *tabWidget = nullptr;
    QTextEdit *commandWindow = nullptr;
    BaseTerminal *currentTab = nullptr;
    QIcon *connectStateIcon = nullptr;
    QIcon *disconnectStateIcon = nullptr;

    bool showToolsBar = true;
    bool showButtonBar = true;
    bool showStatusBar = true;

    QComboBox *commandBarGroup = nullptr;
    QWidget *commandBarLayoutWidget = nullptr;
    QHBoxLayout *commandButtonLayout = nullptr;
    EditCommandGroupDialog *editCommandGroupDialog = nullptr;
    EditCommandButtonDialog *editCommandButtonDialog = nullptr;
    void onActionNewCommandGroup();
    void onActionEditCommandGroup();
    void onActionDeleteCommandGroup();
    void onActionNewCommandButton();
};
#endif // MAINWINDOW_H
