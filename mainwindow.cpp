#include "mainwindow.h"
#include <QTabWidget>
#include <QDebug>
#include <QMessageBox>
#include <QScreen>
#include <QFileDialog>
#include <QTime>
#include <QtCore/QCoreApplication>
#include "resources/forms/ui_mainwindow.h"
#include "common/config/config_manager.h"
#include "common/utils/aes_utils.h"
#include "terminal/local_terminal.h"
#include "terminal/serial_terminal.h"
#include "terminal/ssh_terminal.h"
#include "widget/command_button.h"
#include "widget/edit_session_dialog.h"
#include "widget/edit_command_group_dialog.h"
#include "widget/edit_command_button_dialog.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow), settingDialog(new Settings), sessionManager(new SessionManager),
        editCommandButtonDialog(new EditCommandButtonDialog(this)),
        editCommandGroupDialog(new EditCommandGroupDialog(this)) {

    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    setContextMenuPolicy(Qt::NoContextMenu);

    splitter = new QSplitter(Qt::Vertical, this);
    tabWidget = new QTabWidget(this);
    tabWidget->setTabsClosable(true);
    tabWidget->setMovable(true);
    commandWindow = new QTextEdit(this);
    commandWindow->installEventFilter(this);
    splitter->addWidget(tabWidget);
    splitter->addWidget(commandWindow);
    setCentralWidget(splitter);
    int height = QGuiApplication::primaryScreen()->size().height()
            - ui->menubar->height()
            - ui->toolsBar->height()
            - ui->commandButtonBar->height();
    splitter->setSizes(QList<int>({static_cast<int>(height*0.9), static_cast<int>(height*0.1)}));

    connectStateIcon = new QIcon();
    disconnectStateIcon = new QIcon();
    connectStateIcon->addFile(QString::fromUtf8(":/images/green.png"), QSize(), QIcon::Normal, QIcon::On);
    disconnectStateIcon->addFile(QString::fromUtf8(":/images/red.png"), QSize(), QIcon::Normal, QIcon::On);

    QObject::connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    QObject::connect(tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabCloseRequested);
    QObject::connect(sessionManager, &SessionManager::openSession, this, &MainWindow::onOpenSession);
    QObject::connect(editCommandButtonDialog, &EditCommandButtonDialog::commandButtonChanged, this, &MainWindow::loadCommandBar);
    QObject::connect(editCommandGroupDialog, &EditCommandGroupDialog::commandGroupChanged, this, &MainWindow::onCommandGroupChanged);
    QObject::connect(editCommandGroupDialog, &EditCommandGroupDialog::commandGroupAdd, this, &MainWindow::onCommandGroupAdd);

    actionInit();
    buttonBarInit();
}

MainWindow::~MainWindow() {
    delete ui;
    delete tabWidget;
    delete settingDialog;
    delete sessionManager;
}

void MainWindow::onTabChanged(int index) {
    if (index < 0) {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(false);
        ui->actionLogSession->setEnabled(false);
        ui->actionDisableLogSession->setEnabled(false);
        currentTab = nullptr;
        return;
    }
    currentTab = dynamic_cast<BaseTerminal *>(tabWidget->widget(index));
    if (currentTab->isConnect()) {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->statusBar->showMessage("connect");
    } else {
        ui->actionConnect->setEnabled(true);
        ui->actionDisconnect->setEnabled(false);
        ui->statusBar->showMessage("disconnect");
    }

    if (currentTab->isLoggingSession()) {
        ui->actionLogSession->setEnabled(false);
        ui->actionDisableLogSession->setEnabled(true);
    } else {
        ui->actionLogSession->setEnabled(true);
        ui->actionDisableLogSession->setEnabled(false);
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
    QObject::connect(ui->actionNewLocalShellSession, &QAction::triggered, this,
                     &MainWindow::onActionNewLocalShellSession);
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

    QObject::connect(ui->actionNewCommandGroup, &QAction::triggered, this, &MainWindow::onActionNewCommandGroup);
    QObject::connect(ui->actionEditCommandGroup, &QAction::triggered, this, &MainWindow::onActionEditCommandGroup);
    QObject::connect(ui->actionDeleteCommandGroup, &QAction::triggered, this, &MainWindow::onActionDeleteCommandGroup);
    QObject::connect(ui->actionNewCommandButton, &QAction::triggered, this, &MainWindow::onActionNewCommandButton);

    QObject::connect(ui->actionLogSession, &QAction::triggered, this, &MainWindow::onActionLogSession);
    QObject::connect(ui->actionDisableLogSession, &QAction::triggered, this, &MainWindow::onActionDisableLogSession);
}

void MainWindow::loadCommandBar(const QString &groupName) {
    ConfigManager *conf = ConfigManager::getInstance();

    QLayoutItem *child;
    while ((child = commandButtonLayout->takeAt(0)) != 0) {
        if (child->widget()) {
            child->widget()->setParent(nullptr);
        }
        delete child;
    }

    const char *sectionName = groupName.toStdString().c_str();
    std::vector<std::string> keys = conf->getSectionKeys(sectionName);
    for (const std::string &key : keys) {
        const char *value = conf->getCString(sectionName, key.c_str());
        qDebug() << key.c_str() << ":" << value << endl;
        CommandButton *button = new CommandButton(key.c_str(), value, ui->commandButtonBar);
        QObject::connect(button, &CommandButton::sendCommand, this, &MainWindow::receiveCommand);
        QObject::connect(button, &CommandButton::requestEditCommandButton, this, &MainWindow::onEditCommandButton);
        QObject::connect(button, &CommandButton::requestDeleteCommandButton, this, &MainWindow::onDeleteCommandButton);
        commandButtonLayout->addWidget(button);
    }
}

void MainWindow::onCommandGroupChanged(const QString &groupName) {
    qDebug() << "onCommandGroupChanged" << endl;
    commandBarGroup->setItemText(commandBarGroup->currentIndex(), groupName);
}

void MainWindow::onCommandGroupAdd(const QString &groupName) {
    commandBarGroup->addItem(groupName);
    qDebug() << "onCommandGroupAdd" << endl;
    commandBarGroup->setCurrentText(groupName);
}

void MainWindow::buttonBarInit() {
    ConfigManager *conf = ConfigManager::getInstance();

    commandBarGroup = new QComboBox();
    commandButtonLayout = new QHBoxLayout();
    commandBarLayoutWidget = new QWidget();
    commandBarLayoutWidget->setLayout(commandButtonLayout);
    ui->commandButtonBar->addWidget(commandBarGroup);
    ui->commandButtonBar->addWidget(commandBarLayoutWidget);
    std::vector<std::string> bars = conf->getSectionKeys("command-bars");
    for (const std::string &bar : bars) {
        commandBarGroup->addItem(bar.c_str());
    }

    QObject::connect(commandBarGroup, &QComboBox::currentTextChanged, this, &MainWindow::loadCommandBar);

    loadCommandBar(bars[0].c_str());
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
    QObject::connect(terminal, &BaseTerminal::requestDisconnect, this, &MainWindow::onRequestDisconnect);
    tabWidget->addTab(terminal, *connectStateIcon, session.c_str());
    tabWidget->setCurrentWidget(terminal);
    terminal->setFocus();

}

BaseTerminal *MainWindow::createLocalShellSession(std::string session) {
    LocalTerminal *terminal = new LocalTerminal(this);
    return terminal;
}

BaseTerminal *MainWindow::createSerialSession(std::string session) {
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

BaseTerminal *MainWindow::createSSHSession(std::string session) {
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
        newSerialSessionDialog = new EditSessionDialog("serial", this);
        QObject::connect(newSerialSessionDialog, &EditSessionDialog::sessionListUpdate, sessionManager,
                         &SessionManager::updateSessionList);
    }
    newSerialSessionDialog->show();
}

void MainWindow::onActionNewSSHSession() {
    if (newSSHSessionDialog == nullptr) {
        newSSHSessionDialog = new EditSessionDialog("ssh", this);
        QObject::connect(newSSHSessionDialog, &EditSessionDialog::sessionListUpdate, sessionManager,
                         &SessionManager::updateSessionList);
    }
    newSSHSessionDialog->show();
}

void MainWindow::onActionNewLocalShellSession() {
    if (newLocalShellSessionDialog == nullptr) {
        newLocalShellSessionDialog = new EditSessionDialog("local", this);
        QObject::connect(newLocalShellSessionDialog, &EditSessionDialog::sessionListUpdate, sessionManager,
                         &SessionManager::updateSessionList);
    }
    newLocalShellSessionDialog->show();
}

void MainWindow::onRequestDisconnect(BaseTerminal *terminal) {
    if (terminal == currentTab) {
        onActionDisconnect();
    } else {
        int index = tabWidget->indexOf(terminal);
        terminal->disconnect();
        tabWidget->setTabIcon(index, *disconnectStateIcon);
    }
}

void MainWindow::onActionNewCommandGroup() {
    qDebug() << "onActionNewCommandGroup" << endl;
    editCommandGroupDialog->clearField();
    editCommandGroupDialog->show();
}

void MainWindow::onActionEditCommandGroup() {
    qDebug() << "onActionEditCommandGroup" << endl;
    std::string groupName = commandBarGroup->currentText().toStdString();
    editCommandGroupDialog->clearField();
    editCommandGroupDialog->setField(groupName);
    editCommandGroupDialog->show();
}

void MainWindow::onActionDeleteCommandGroup() {
    qDebug() << "onActionDeleteCommandGroup" << endl;
    std::string groupName = commandBarGroup->currentText().toStdString();
    QMessageBox msgBox;
    msgBox.setMinimumSize(640, 480);
    msgBox.setText("warning:");
    msgBox.setInformativeText(("are you sure to delete command group: " + groupName + " ?").c_str());
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok) {
        ConfigManager *conf = ConfigManager::getInstance();
        conf->deleteSection(groupName.c_str());
        conf->deleteKey("command-bars", groupName.c_str());
        commandBarGroup->removeItem(commandBarGroup->currentIndex());
    }

}

void MainWindow::onActionNewCommandButton() {
    qDebug() << "onActionNewCommandButton" << endl;
    std::string groupName = commandBarGroup->currentText().toStdString();
    editCommandButtonDialog->clearField();
    editCommandButtonDialog->setField(groupName, "", "");
    editCommandButtonDialog->show();
}

void MainWindow::onEditCommandButton(const QString &commandName, const QString &command) {
    std::string groupName = commandBarGroup->currentText().toStdString();
    editCommandButtonDialog->clearField();
    editCommandButtonDialog->setField(groupName, commandName.toStdString(), command.toStdString());
    editCommandButtonDialog->show();
}

void MainWindow::onDeleteCommandButton(const QString &commandName) {
    qDebug() << "onDeleteCommandButton" << endl;
    std::string groupName = commandBarGroup->currentText().toStdString();
    QMessageBox msgBox;
    msgBox.setMinimumSize(640, 480);
    msgBox.setText("warning:");
    msgBox.setInformativeText(("are you sure to delete command button: " + commandName.toStdString() + " ?").c_str());
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok) {
        ConfigManager *conf = ConfigManager::getInstance();
        conf->deleteKey(groupName.c_str(), commandName.toStdString().c_str());
    }

    loadCommandBar(groupName.c_str());
}

std::string MainWindow::selectLogPath() {
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle("select log file");
    fileDialog->setDirectory(".");
    fileDialog->setViewMode(QFileDialog::Detail);
    if(fileDialog->exec()) {
        QStringList fileNames = fileDialog->selectedFiles();
        if (!fileNames.empty()) {
            return fileNames.first().toStdString();
        }
    }
    return "";
}

void MainWindow::onActionDisableLogSession() {
    if (currentTab == nullptr) {
        return;
    }
    currentTab->disableLogSession();
    ui->actionLogSession->setEnabled(true);
    ui->actionDisableLogSession->setEnabled(false);
}

void MainWindow::onActionLogSession() {
    if (currentTab == nullptr) {
        return;
    }

    std::string logPath = selectLogPath();
    if (logPath.empty()) {
        return;
    }
    currentTab->logSession(logPath);
    ui->actionLogSession->setEnabled(false);
    ui->actionDisableLogSession->setEnabled(true);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e) {
    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *event = static_cast<QKeyEvent *>(e);
        if (event->key() == Qt::Key_Return) {
            if (event->modifiers() & Qt::ControlModifier) {
                commandWindow->append("");
            } else {
                if (currentTab == nullptr) {
                    return false;
                }
                currentTab->sendText(commandWindow->toPlainText() + "\r");
                commandWindow->clear();
            }
            return true;
        }
    }
    return false;
}

