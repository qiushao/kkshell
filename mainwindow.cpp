#include "mainwindow.h"
#include <QTabWidget>
#include <QDebug>
#include "resources/forms/ui_mainwindow.h"
#include "common/config/config_manager.h"
#include "terminal/local_terminal.h"
#include "terminal/serial_terminal.h"
#include "terminal/ssh_terminal.h"
#include "widget/command_button.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , settingDialog(new Settings)
    , sessionManager(new SessionManager)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/images/application.png"));
    setWindowState(Qt::WindowMaximized);
    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    setCentralWidget(tabWidget);

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
    currentTab = dynamic_cast<BaseTerminal *>(tabWidget->widget(index));
    if(currentTab->isConnect()) {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->statusbar->showMessage("connect");
    } else {
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
        ui->statusbar->showMessage("disconnect");
    }
}

void MainWindow::onTabCloseRequested(int index) {
    BaseTerminal *tab = dynamic_cast<BaseTerminal *>(tabWidget->widget(index));
    tab->disconnect();
    tabWidget->removeTab(index);
    delete tab;
}

void MainWindow::onActionCopy() {
    currentTab->copyClipboard();
}

void MainWindow::onActionPaste() {
    currentTab->pasteClipboard();
}

void MainWindow::onActionClear() {
    currentTab->clear();
}

void MainWindow::onActionFind() {
    currentTab->toggleShowSearchBar();
}

void MainWindow::onActionConnect() {
    currentTab->connect();
    if (currentTab->isConnect()) {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->statusbar->showMessage("connect");
    }
}

void MainWindow::onActionDisconnect() {
    currentTab->disconnect();
    if (!currentTab->isConnect()) {
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
        ui->statusbar->showMessage("disconnect");
    }
}

void MainWindow::onActionExit() {
    exit(0);
}

void MainWindow::onActionSetting() {
    settingDialog->show();
}

void MainWindow::onActionNewSession() {
    sessionManager->show();
}

void MainWindow::actionInit() {
    QObject::connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::onActionSetting);
    QObject::connect(ui->actionNewSession, &QAction::triggered, this, &MainWindow::onActionNewSession);
    QObject::connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onActionExit);

    QObject::connect(ui->actionConnect, &QAction::triggered, this, &MainWindow::onActionConnect);
    QObject::connect(ui->actionDisconnect, &QAction::triggered, this, &MainWindow::onActionDisconnect);
    QObject::connect(ui->actionCopy, &QAction::triggered, this, &MainWindow::onActionCopy);
    QObject::connect(ui->actionPaste, &QAction::triggered, this, &MainWindow::onActionPaste);
    QObject::connect(ui->actionClear, &QAction::triggered, this, &MainWindow::onActionClear);
    QObject::connect(ui->actionFind, &QAction::triggered, this, &MainWindow::onActionFind);
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
    tabWidget->addTab(terminal, session.c_str());
    terminal->connect();
    tabWidget->setCurrentWidget(terminal);
    terminal->focusWidget();
}

BaseTerminal* MainWindow::createLocalShellSession(std::string session) {
    LocalTerminal *terminal = new LocalTerminal(this);
    return terminal;
}

BaseTerminal* MainWindow::createSerialSession(std::string session) {
    ConfigManager *conf = ConfigManager::getInstance();
    SerialSettings settings;
    settings.name = conf->getString(session.c_str(), "dev").c_str();
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
    sshSettings.passwd = conf->getString(session.c_str(), "passwd");
    SSHTerminal *terminal = new SSHTerminal(sshSettings, this);
    return terminal;
}
