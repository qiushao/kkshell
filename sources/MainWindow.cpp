//
// Created by qiushao on 2020/11/1.
//

#include <QDebug>
#include <QApplication>
#include <QFileDialog>
#include "MainWindow.h"
#include "SettingManager.h"
#include "widgets/terminal/LocalTerminal.h"
#include "widgets/terminal/SerialTerminal.h"
#include "widgets/terminal/SSHTerminal.h"
#include "widgets/command/CommandButton.h"
#include "widgets/SettingDialog.h"

MainWindow::MainWindow(QWidget *parent) {
    setWindowState(Qt::WindowMaximized);
    setContextMenuPolicy(Qt::NoContextMenu);
    SettingManager *settingManager = SettingManager::getInstance();
    QIcon windowIcon(":/images/application.png");
    setWindowIcon(windowIcon);

    initIcons();
    initActions();
    initMenu();
    initToolbar();
    initTableWidget();
    initSessionList();
    initCommandWindow();
    initCommandBar();
}

MainWindow::~MainWindow() {

}

void MainWindow::initSessionList() {
    _sessionDock = new QDockWidget(tr("Session Manager"), this);
    _sessionListWidget = new SessionListWidget(this);
    _sessionDock->setWidget(_sessionListWidget);
    _sessionDock->setFeatures(QDockWidget::DockWidgetMovable);
    _sessionDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    _sessionDock->setMinimumWidth(220);
    _sessionDock->setMaximumWidth(480);
    addDockWidget(Qt::LeftDockWidgetArea, _sessionDock);

    _sessionListWidget->loadSessionList();

    connect(_sessionListWidget, &SessionListWidget::requestOpenSession, this, &MainWindow::onRequestOpenSession);
}

void MainWindow::initTableWidget() {
    _tabWidget = new QTabWidget(this);
    _tabWidget->setTabsClosable(true);
    _tabWidget->setMovable(true);
    setCentralWidget(_tabWidget);

    QObject::connect(_tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    QObject::connect(_tabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabCloseRequested);
}

void MainWindow::initCommandWindow() {
    _commandWindowDock = new QDockWidget(tr("Command Window"), this);
    _commandWindowWidget = new QTextEdit(this);
    _commandWindowDock->setWidget(_commandWindowWidget);
    _commandWindowDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    _commandWindowWidget->setMinimumHeight(50);
    addDockWidget(Qt::BottomDockWidgetArea, _commandWindowDock);
    resizeDocks({_commandWindowDock}, {50}, Qt::Vertical);
    _commandWindowWidget->installEventFilter(this);

    //disable title bar
    QWidget *titleBar = _commandWindowDock->titleBarWidget();
    auto *emptyWidget = new QWidget();
    _commandWindowDock->setTitleBarWidget(emptyWidget);
    delete titleBar;
}

void MainWindow::initCommandBar() {
    _commandBar = new CommandToolBar(this);
    _commandBar->setMovable(false);
    addToolBar(Qt::BottomToolBarArea, _commandBar);
    connect(_commandBar, &CommandToolBar::sendCommand, this, &MainWindow::receiveCommand);
}

void MainWindow::receiveCommand(const Command &command) {
    if (_currentTab == nullptr) {
        return;
    }
    QString str = command.commandString.c_str();
    str.replace(QString("\\r"), QString("\r"));
    _currentTab->sendText(str);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e) {
    if (e->type() == QEvent::KeyPress) {
        auto *event = dynamic_cast<QKeyEvent *>(e);
        if (event->key() == Qt::Key_Return) {
            if (event->modifiers() & Qt::ControlModifier) {
                _commandWindowWidget->append("");
            } else {
                if (_currentTab == nullptr) {
                    return false;
                }
                _currentTab->sendText(_commandWindowWidget->toPlainText() + "\r");
                _commandWindowWidget->clear();
            }
            return true;
        }
    }
    return false;
}

void MainWindow::initIcons() {
    _settingsIcon = new QIcon(":/images/setting.png");
    _connectIcon = new QIcon(":/images/connect.png");
    _disconnectIcon = new QIcon(":/images/disconnect.png");
    _connectStateIcon = new QIcon(":/images/connect_state.png");
    _disconnectStateIcon = new QIcon(":/images/disconnect_state.png");
    _exitIcon = new QIcon(":/images/exit.png");

    _copySelectedIcon = new QIcon(":/images/copy.png");
    _copyAllIcon = new QIcon(":/images/copy_all.png");
    _pasteIcon = new QIcon(":/images/paste.png");
    _findIcon = new QIcon(":/images/find.png");
    _clearScreenIcon = new QIcon(":/images/clear.png");

    _toggleOnIcon = new QIcon(":/images/toggle_on.png");
    _toggleOffIcon = new QIcon(":/images/toggle_off.png");
}

void MainWindow::initActions() {
    _settingsAction = new QAction(*_settingsIcon, tr("Setting"), this);
    connect(_settingsAction, &QAction::triggered, this, &MainWindow::onSettingsAction);

    _toggleSaveLogAction = new QAction(*_toggleOffIcon, tr("Save Session Log"), this);
    _toggleSaveLogAction->setEnabled(false);
    connect(_toggleSaveLogAction, &QAction::triggered, this, &MainWindow::onToggleSaveLogAction);

    _toggleSaveHexLogAction = new QAction(*_toggleOffIcon, tr("Save Session Hex Log"), this);
    _toggleSaveHexLogAction->setEnabled(false);
    connect(_toggleSaveHexLogAction, &QAction::triggered, this, &MainWindow::onToggleSaveHexLogAction);

    _connectAction = new QAction(*_connectIcon, tr("Connect"), this);
    _connectAction->setEnabled(false);
    connect(_connectAction, &QAction::triggered, this, &MainWindow::onConnectAction);

    _disConnectAction = new QAction(*_disconnectIcon, tr("Disconnect"), this);
    _disConnectAction->setEnabled(false);
    connect(_disConnectAction, &QAction::triggered, this, &MainWindow::onDisconnectAction);

    _exitAction = new QAction(*_exitIcon, tr("Exit"), this);
    connect(_exitAction, &QAction::triggered, this, &MainWindow::onExitAction);


    _copySelectedAction = new QAction(*_copySelectedIcon, tr("Copy Selected"), this);
    connect(_copySelectedAction, &QAction::triggered, this, &MainWindow::onCopySelectedAction);
    _copySelectedAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+C", nullptr));

    _copyAllAction = new QAction(*_copyAllIcon, tr("Copy All"), this);
    connect(_copyAllAction, &QAction::triggered, this, &MainWindow::onCopyAllAction);
    _copyAllAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+A", nullptr));

    _pasteAction = new QAction(*_pasteIcon, tr("Paste"), this);
    connect(_pasteAction, &QAction::triggered, this, &MainWindow::onPasteAction);
    _pasteAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+V", nullptr));

    _findAction = new QAction(*_findIcon, tr("Find"), this);
    connect(_findAction, &QAction::triggered, this, &MainWindow::onFindAction);
    _findAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+F", nullptr));

    _clearScreenAction = new QAction(*_clearScreenIcon, tr("Clear Screen"), this);
    connect(_clearScreenAction, &QAction::triggered, this, &MainWindow::onClearScreenAction);
    _clearScreenAction->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+X", nullptr));

    _toggleToolbarAction = new QAction(*_toggleOnIcon, tr("Toolbar"), this);
    connect(_toggleToolbarAction, &QAction::triggered, this, &MainWindow::onToggleToolbarAction);

    _toggleSessionManagerAction = new QAction(*_toggleOnIcon, tr("Session Manager"), this);
    connect(_toggleSessionManagerAction, &QAction::triggered, this, &MainWindow::onToggleSessionManagerAction);

    _toggleCommandWindowAction = new QAction(*_toggleOnIcon, tr("Command Window"), this);
    connect(_toggleCommandWindowAction, &QAction::triggered, this, &MainWindow::onToggleCommandWindowAction);

    _toggleCommandButtonAction = new QAction(*_toggleOnIcon, tr("Command Button"), this);
    connect(_toggleCommandButtonAction, &QAction::triggered, this, &MainWindow::onToggleCommandButtonAction);
}

void MainWindow::initMenu() {
    _mainMenuBar = new QMenuBar(this);
    setMenuBar(_mainMenuBar);

    _fileMenu = new QMenu(tr("File"), _mainMenuBar);
    _mainMenuBar->addAction(_fileMenu->menuAction());
    _fileMenu->addAction(_settingsAction);
    _fileMenu->addAction(_toggleSaveLogAction);
    _fileMenu->addAction(_toggleSaveHexLogAction);
    _fileMenu->addAction(_connectAction);
    _fileMenu->addAction(_disConnectAction);
    _fileMenu->addAction(_exitAction);

    _editMenu = new QMenu(tr("Edit"), _mainMenuBar);
    _mainMenuBar->addAction(_editMenu->menuAction());
    _editMenu->addAction(_copySelectedAction);
    _editMenu->addAction(_copyAllAction);
    _editMenu->addAction(_pasteAction);
    _editMenu->addAction(_findAction);
    _editMenu->addAction(_clearScreenAction);

    _viewMenu = new QMenu(tr("View"), _mainMenuBar);
    _mainMenuBar->addAction(_viewMenu->menuAction());
    _viewMenu->addAction(_toggleToolbarAction);
    _viewMenu->addAction(_toggleSessionManagerAction);
    _viewMenu->addAction(_toggleCommandWindowAction);
    _viewMenu->addAction(_toggleCommandButtonAction);

    _helpMenu = new QMenu(tr("Help"), _mainMenuBar);
    _mainMenuBar->addAction(_helpMenu->menuAction());
}

void MainWindow::initToolbar() {
    _toolBar = new QToolBar(this);
    _toolBar->setMovable(false);
    addToolBar(Qt::TopToolBarArea, _toolBar);
    _toolBar->addAction(_settingsAction);
    _toolBar->addSeparator();
    _toolBar->addAction(_connectAction);
    _toolBar->addAction(_disConnectAction);
    _toolBar->addSeparator();
    _toolBar->addAction(_copyAllAction);
    _toolBar->addAction(_copySelectedAction);
    _toolBar->addAction(_pasteAction);
    _toolBar->addSeparator();
    _toolBar->addAction(_findAction);
    _toolBar->addAction(_clearScreenAction);
}

void MainWindow::onSettingsAction() {
    SettingDialog settingDialog(this);
    settingDialog.exec();
}

std::string MainWindow::selectLogPath() {
    auto *fileDialog = new QFileDialog(this);
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

void MainWindow::onToggleSaveLogAction() {
    if (_currentTab == nullptr) {
        return;
    }

    if (_currentTab->isLoggingSession()) {
        _currentTab->disableLogSession();
        _toggleSaveLogAction->setIcon(*_toggleOffIcon);
    } else {
        std::string logPath = selectLogPath();
        if (logPath.empty()) {
            return;
        }
        _currentTab->logSession(logPath);
        _toggleSaveLogAction->setIcon(*_toggleOnIcon);
    }
}

void MainWindow::onToggleSaveHexLogAction() {
    if (_currentTab == nullptr) {
        return;
    }

    if (_currentTab->isLoggingHexSession()) {
        _currentTab->disableLogHexSession();
        _toggleSaveHexLogAction->setIcon(*_toggleOffIcon);
    } else {
        std::string logPath = selectLogPath();
        if (logPath.empty()) {
            return;
        }
        _currentTab->logHexSession(logPath);
        _toggleSaveHexLogAction->setIcon(*_toggleOnIcon);
    }
}

void MainWindow::onConnectAction() {
    if (_currentTab == nullptr) {
        return;
    }

    _currentTab->connect();
    if (_currentTab->isConnect()) {
        _connectAction->setEnabled(false);
        _disConnectAction->setEnabled(true);
    }

    _tabWidget->setTabIcon(_tabWidget->currentIndex(), *_connectStateIcon);
}

void MainWindow::onDisconnectAction() {
    if (_currentTab == nullptr) {
        return;
    }

    _currentTab->disconnect();
    if (!_currentTab->isConnect()) {
        _connectAction->setEnabled(true);
        _disConnectAction->setEnabled(false);
    }

    _tabWidget->setTabIcon(_tabWidget->currentIndex(), *_disconnectStateIcon);
}

void MainWindow::onExitAction() {
    BaseTerminal *terminal = nullptr;
    while (nullptr != (terminal = dynamic_cast<BaseTerminal *>(_tabWidget->currentWidget()))) {
        terminal->disconnect();
    }
    exit(0);
}

void MainWindow::onCopySelectedAction() {
    if (_currentTab != nullptr) {
        _currentTab->copyClipboard();
    }
}

void MainWindow::onCopyAllAction() {
    _currentTab->setSelectionStart(0, 0);
    _currentTab->setSelectionEnd(_currentTab->screenLinesCount(), _currentTab->screenColumnsCount());
    _currentTab->copyClipboard();
}

void MainWindow::onPasteAction() {
    if (_currentTab != nullptr) {
        _currentTab->pasteClipboard();
    }
}

void MainWindow::onFindAction() {
    if (_currentTab != nullptr) {
        _currentTab->toggleShowSearchBar();
    }
}

void MainWindow::onClearScreenAction() {
    if (_currentTab != nullptr) {
        _currentTab->clear();
    }
}

void MainWindow::onToggleToolbarAction() {
    if (_toolBar->isHidden()) {
        _toolBar->show();
        _toggleToolbarAction->setIcon(*_toggleOnIcon);
    } else {
        _toolBar->hide();
        _toggleToolbarAction->setIcon(*_toggleOffIcon);
    }
}

void MainWindow::onToggleSessionManagerAction() {
    if (_sessionDock->isHidden()) {
        _sessionDock->show();
        _toggleSessionManagerAction->setIcon(*_toggleOnIcon);
    } else {
        _sessionDock->hide();
        _toggleSessionManagerAction->setIcon(*_toggleOffIcon);
    }
}

void MainWindow::onToggleCommandWindowAction() {
    if (_commandWindowDock->isHidden()) {
        _commandWindowDock->show();
        _toggleCommandWindowAction->setIcon(*_toggleOnIcon);
    } else {
        _commandWindowDock->hide();
        _toggleCommandWindowAction->setIcon(*_toggleOffIcon);
    }
}

void MainWindow::onToggleCommandButtonAction() {
    if (_commandBar->isHidden()) {
        _commandBar->show();
        _toggleCommandButtonAction->setIcon(*_toggleOnIcon);
    } else {
        _commandBar->hide();
        _toggleCommandButtonAction->setIcon(*_toggleOffIcon);
    }
}

void MainWindow::onRequestOpenSession(const Session &session) {
    BaseTerminal *terminal = nullptr;
    if (session.sessionType == SESSION_TYPE::LOCAL_SHELL) {
        terminal = new LocalTerminal(this);
    } else if (session.sessionType == SESSION_TYPE::SERIAL) {
        terminal = new SerialTerminal(session, this);
    } else if (session.sessionType == SESSION_TYPE::SSH2) {
        terminal = new SSHTerminal(session, this);
    } else {
        qDebug() << "unknown session type!!" << endl;
        return;
    }

    terminal->connect();
    QObject::connect(terminal, &BaseTerminal::requestDisconnect, this, &MainWindow::onRequestDisconnect);
    _tabWidget->addTab(terminal, *_connectStateIcon, session.sessionName.c_str());
    _tabWidget->setCurrentWidget(terminal);
    terminal->setFocus();
}

void MainWindow::onRequestDisconnect(BaseTerminal *terminal) {
    if (terminal == _currentTab) {
        onDisconnectAction();
    } else {
        int index = _tabWidget->indexOf(terminal);
        terminal->disconnect();
        _tabWidget->setTabIcon(index, *_disconnectStateIcon);
    }
}

void MainWindow::onTabChanged(int index) {
    qDebug() << "onTabChanged, index = " << index;
    if (index < 0) {
        _connectAction->setEnabled(false);
        _disConnectAction->setEnabled(false);
        _toggleSaveLogAction->setEnabled(false);
        _toggleSaveHexLogAction->setEnabled(false);
        _currentTab = nullptr;
        return;
    }

    _currentTab = dynamic_cast<BaseTerminal *>(_tabWidget->widget(index));
    if (_currentTab->isConnect()) {
        _connectAction->setEnabled(false);
        _disConnectAction->setEnabled(true);
    } else {
        _connectAction->setEnabled(true);
        _disConnectAction->setEnabled(false);
    }

    _toggleSaveLogAction->setEnabled(true);
    _toggleSaveHexLogAction->setEnabled(true);
    if (_currentTab->isLoggingSession()) {
        _toggleSaveLogAction->setIcon(*_toggleOnIcon);
    } else {
        _toggleSaveLogAction->setIcon(*_toggleOffIcon);
    }

}

void MainWindow::onTabCloseRequested(int index) {
    auto *tab = dynamic_cast<BaseTerminal *>(_tabWidget->widget(index));
    tab->disconnect();
    _tabWidget->removeTab(index);
    delete tab;
}


