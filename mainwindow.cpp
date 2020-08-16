#include "mainwindow.h"
#include <QTabWidget>
#include <QDebug>
#include "resources/forms/ui_mainwindow.h"
#include "common/config/config_manager.h"
#include "common/utils/aes_utils.h"
#include "terminal/local_terminal.h"
#include "terminal/serial_terminal.h"
#include "terminal/ssh_terminal.h"
#include "widget/command_button.h"
#include "widget/new_session_dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settingDialog(new Settings)
    , sessionManager(new SessionManager)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    tabWidget->setMovable(true);
    setCentralWidget(tabWidget);

    connectStateIcon = new QIcon();
    disconnectStateIcon = new QIcon();
    connectStateIcon->addFile(QString::fromUtf8(":/images/green.png"), QSize(), QIcon::Normal, QIcon::On);
    disconnectStateIcon->addFile(QString::fromUtf8(":/images/red.png"), QSize(), QIcon::Normal, QIcon::On);

    QObject::connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    QObject::connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabCloseRequested);
    QObject::connect(sessionManager, &SessionManager::openSession, this, &MainWindow::onOpenSession);

    actionInit();
    buttonBarInit();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tabWidget;
    delete settingDialog;
    delete sessionManager;
}

void MainWindow::onTabChanged(int index) {
    if (index < 0) {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(false);
        return;
    }
    currentTab = dynamic_cast<BaseTerminal *>(tabWidget->widget(index));
    if(currentTab->isConnect()) {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->statusBar->showMessage("connect");
    } else {
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
        ui->statusBar->showMessage("disconnect");
    }
}

void MainWindow::onTabCloseRequested(int index) {
    BaseTerminal *tab = dynamic_cast<BaseTerminal *>(tabWidget->widget(index));
    tab->disconnect();
    tabWidget->removeTab(index);
    delete tab;
}

void MainWindow::onActionSelectAll() {
    if (currentTab != nullptr) {
        qDebug() << "onActionSelectAll" << endl;
        currentTab->setSelectionStart(0, 0);
        currentTab->setSelectionEnd(currentTab->screenLinesCount(), currentTab->screenColumnsCount());
        currentTab->copyClipboard();
    }
}

void MainWindow::onActionCopy() {
    if (currentTab != nullptr) {
        currentTab->copyClipboard();
    }
}

void MainWindow::onActionPaste() {
    if (currentTab != nullptr) {
        currentTab->pasteClipboard();
    }
}

void MainWindow::onActionClear() {
    if (currentTab != nullptr) {
        currentTab->clear();
    }
}

void MainWindow::onActionFind() {
    if (currentTab != nullptr) {
        currentTab->toggleShowSearchBar();
    }
}

void MainWindow::onActionConnect() {
    if (currentTab == nullptr) {
        return;
    }

    currentTab->connect();
    if (currentTab->isConnect()) {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->statusBar->showMessage("connect");
    }

    tabWidget->setTabIcon(tabWidget->currentIndex(), *connectStateIcon);
}

void MainWindow::onActionDisconnect() {
    if (currentTab == nullptr) {
        return;
    }
    currentTab->disconnect();
    if (!currentTab->isConnect()) {
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
        ui->statusBar->showMessage("disconnect");
    }

    tabWidget->setTabIcon(tabWidget->currentIndex(), *disconnectStateIcon);
}

void MainWindow::onActionExit() {
    exit(0);
}

void MainWindow::onActionSetting() {
    settingDialog->show();
}

void MainWindow::onActionOpenSession() {
    sessionManager->show();
}

void MainWindow::onActionShowToolsBar() {
    if (showToolsBar) {
        ui->toolsBar->hide();
        showToolsBar = false;
        ui->actionShowToolsBar->setIconVisibleInMenu(false);
    } else {
        ui->toolsBar->show();
        showToolsBar = true;
        ui->actionShowToolsBar->setIconVisibleInMenu(true);
    }
}

void MainWindow::onActionShowButtonBar() {
    if (showButtonBar) {
        ui->commandButtonBar->hide();
        showButtonBar = false;
        ui->actionShowButtonBar->setIconVisibleInMenu(false);
    } else {
        ui->commandButtonBar->show();
        showButtonBar = true;
        ui->actionShowButtonBar->setIconVisibleInMenu(true);
    }
}

void MainWindow::onActionShowStatusBar() {
    if (showStatusBar) {
        ui->statusBar->hide();
        showStatusBar = false;
        ui->actionShowStatusBar->setIconVisibleInMenu(false);
    } else {
        ui->statusBar->show();
        showStatusBar = true;
        ui->actionShowStatusBar->setIconVisibleInMenu(true);
    }
}

void MainWindow::actionInit() {
    QObject::connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::onActionSetting);
    QObject::connect(ui->actionOpenSession, &QAction::triggered, this, &MainWindow::onActionOpenSession);
    QObject::connect(ui->actionNewLocalShellSession, &QAction::triggered, this, &MainWindow::onActionNewLocalShellSession);
    QObject::connect(ui->actionNewSSHSession, &QAction::triggered, this, &MainWindow::onActionNewSSHSession);
    QObject::connect(ui->actionNewSerialSession, &QAction::triggered, this, &MainWindow::onActionNewSerialSession);
    QObject::connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onActionExit);

    QObject::connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::onActionConnect);
    QObject::connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::onActionDisconnect);
    QObject::connect(ui->actionSelectAll, &QAction::triggered, this, &MainWindow::onActionSelectAll);
    QObject::connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::onActionCopy);
    QObject::connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::onActionPaste);
    QObject::connect(ui->actionClear, &QAction::triggered, this, &MainWindow::onActionClear);
    QObject::connect(ui->actionFind, &QAction::triggered, this, &MainWindow::onActionFind);

    QObject::connect(ui->actionShowToolsBar, &QAction::triggered, this, &MainWindow::onActionShowToolsBar);
    QObject::connect(ui->actionShowButtonBar, &QAction::triggered, this, &MainWindow::onActionShowButtonBar);
    QObject::connect(ui->actionShowStatusBar, &QAction::triggered, this, &MainWindow::onActionShowStatusBar);
}

void MainWindow::buttonBarInit() {
    ConfigManager *conf = ConfigManager::getInstance();
    const char *sectionName = "button_bar";
    std::vector<std::string> keys = conf->getSectionKeys(sectionName);
    for (std::string key : keys) {
        std::string title = key.substr(3);
        const char *value = conf->getCString(sectionName, key.c_str());
        qDebug() << key.c_str() << ":" << value << endl;
        CommandButton *button = new CommandButton(title.c_str(), value, ui->commandButtonBar);
        connect(button, &CommandButton::sendCommand, this, &MainWindow::receiveCommand);
        ui->commandButtonBar->addWidget(button);
    }
}

void MainWindow::receiveCommand(const QString &command) {
    if (currentTab == nullptr) {
        return;
    }
    QString str = command;
    str.replace(QString("\\r"), QString("\r"));
    currentTab->sendText(str);
}

void MainWindow::onOpenSession(std::string session) {
    ConfigManager *conf = ConfigManager::getInstance();
    BaseTerminal *terminal = nullptr;
    std::string type = conf->getString("sessions", session.c_str());
    if (type == "local") {
        terminal = createLocalShellSession(session);
    } else if (type == "serial") {
        terminal = createSerialSession(session);
    } else if (type == "ssh") {
        terminal = createSSHSession(session);
    } else {
        qDebug() << "unknown session type!!" << endl;
        return;
    }
    terminal->connect();
    tabWidget->addTab(terminal, *connectStateIcon, session.c_str());
    tabWidget->setCurrentWidget(terminal);
    terminal->setFocus();
}

BaseTerminal* MainWindow::createLocalShellSession(std::string session) {
    LocalTerminal *terminal = new LocalTerminal(this);
    return terminal;
}

BaseTerminal* MainWindow::createSerialSession(std::string session) {
    ConfigManager *conf = ConfigManager::getInstance();
    SerialSettings settings;
    settings.name = conf->getString(session.c_str(), "dev").c_str();
    settings.name = "/dev/" + settings.name;
    settings.baudRate = conf->getInt(session.c_str(), "baudRate");
    settings.dataBits = static_cast<QSerialPort::DataBits>(conf->getInt(session.c_str(), "dataBits"));
    settings.parity = static_cast<QSerialPort::Parity>(conf->getInt(session.c_str(), "parity"));
    settings.stopBits = static_cast<QSerialPort::StopBits>(conf->getInt(session.c_str(), "stopBits"));
    settings.flowControl = static_cast<QSerialPort::FlowControl>(conf->getInt(session.c_str(), "flowControl"));
    SerialTerminal *terminal = new SerialTerminal(settings, this);
    return terminal;
}

BaseTerminal* MainWindow::createSSHSession(std::string session) {
    ConfigManager *conf = ConfigManager::getInstance();
    SSHSettings sshSettings;
    sshSettings.host = conf->getString(session.c_str(), "host");
    sshSettings.port = conf->getInt(session.c_str(), "port");
    sshSettings.user = conf->getString(session.c_str(), "user");
    sshSettings.authType = conf->getString(session.c_str(), "authType");
    std::string aesPasswd = conf->getString(session.c_str(), "passwd");
    sshSettings.passwd = AESUtils::aes_decrypt(aesPasswd);
    sshSettings.keyFile = conf->getString(session.c_str(), "keyFile");
    SSHTerminal *terminal = new SSHTerminal(sshSettings, this);
    return terminal;
}

void MainWindow::onActionNewSerialSession() {
    if (newSerialSessionDialog == nullptr) {
        newSerialSessionDialog = new NewSessionDialog("serial", this);
        QObject::connect(newSerialSessionDialog, &NewSessionDialog::sessionListUpdate, sessionManager, &SessionManager::updateSessionList);
    }
    newSerialSessionDialog->show();
}

void MainWindow::onActionNewSSHSession() {
    if (newSSHSessionDialog == nullptr) {
        newSSHSessionDialog = new NewSessionDialog("ssh", this);
        QObject::connect(newSSHSessionDialog, &NewSessionDialog::sessionListUpdate, sessionManager, &SessionManager::updateSessionList);
    }
    newSSHSessionDialog->show();
}

void MainWindow::onActionNewLocalShellSession() {
    if (newLocalShellSessionDialog == nullptr) {
        newLocalShellSessionDialog = new NewSessionDialog("local", this);
        QObject::connect(newLocalShellSessionDialog, &NewSessionDialog::sessionListUpdate, sessionManager, &SessionManager::updateSessionList);
    }
    newLocalShellSessionDialog->show();
}




