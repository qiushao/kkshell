//
// Created by mint on 2020/8/10.
//

#include "session_manager.h"
#include <QDebug>
#include <QHeaderView>
#include "widget/edit_session_dialog.h"
#include "common/config/config_manager.h"
#include "resources/forms/ui_session_manager.h"

SessionManager::SessionManager(QWidget *parent) : QMainWindow(parent), ui(new Ui::SessionManager) {
    ui->setupUi(this);
    sessionListView = new QTableWidget(this);
    setCentralWidget(sessionListView);


    QStringList listHeader;
    listHeader << "title" << "host/dev" << "port/baudRate";
    sessionListView->setColumnCount(3);
    sessionListView->setHorizontalHeaderLabels(listHeader);
    sessionListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    sessionListView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //所有列都扩展自适应宽度，填充充满整个控件
    QHeaderView *headerView = sessionListView->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::Stretch);

    QObject::connect(sessionListView, &QTableWidget::itemDoubleClicked, this, &SessionManager::onItemDoubleClicked);
    QObject::connect(ui->actionSessionEdit, &QAction::triggered, this, &SessionManager::onActionEditSession);
    QObject::connect(ui->actionSessionDelete, &QAction::triggered, this, &SessionManager::onActionDeleteSession);

    fillSessionList();
}

SessionManager::~SessionManager() {

}

void SessionManager::fillSessionList() {
    ConfigManager *conf = ConfigManager::getInstance();
    std::vector<std::string> sessions = conf->getSectionKeys("sessions");
    sessionListView->setRowCount(sessions.size());
    int row = 0;
    for (std::string session : sessions) {
        std::string title = session;
        std::string type = conf->getString("sessions", title.c_str());
        if (type == "local") {
            addLocalShellSessionToList(session, row);
        } else if (type == "serial") {
            addSerialSessionToList(session, row);
        } else if (type == "ssh") {
            addSSHSessionToList(session, row);
        } else {
            qDebug() << "unknown session type!!" << endl;
        }
        row++;
    }
}

void SessionManager::onItemDoubleClicked(QTableWidgetItem *item) {
    hide();
    std::string sessionName = getSelectSessionName();
    emit openSession(sessionName);
}

void SessionManager::addLocalShellSessionToList(std::string session, int row) {
    sessionListView->setItem(row, 0, new QTableWidgetItem(session.c_str()));
    sessionListView->setItem(row, 1, new QTableWidgetItem("local"));
    sessionListView->setItem(row, 2, new QTableWidgetItem("local"));
}

void SessionManager::addSerialSessionToList(std::string session, int row) {
    ConfigManager *conf = ConfigManager::getInstance();
    std::string dev = conf->getString(session.c_str(), "dev");
    std::string baudRate = conf->getString(session.c_str(), "baudRate");

    sessionListView->setItem(row, 0, new QTableWidgetItem(session.c_str()));
    sessionListView->setItem(row, 1, new QTableWidgetItem(dev.c_str()));
    sessionListView->setItem(row, 2, new QTableWidgetItem(baudRate.c_str()));
}

void SessionManager::addSSHSessionToList(std::string session, int row) {
    ConfigManager *conf = ConfigManager::getInstance();
    std::string host = conf->getString(session.c_str(), "host");
    std::string port = conf->getString(session.c_str(), "port");

    sessionListView->setItem(row, 0, new QTableWidgetItem(session.c_str()));
    sessionListView->setItem(row, 1, new QTableWidgetItem(host.c_str()));
    sessionListView->setItem(row, 2, new QTableWidgetItem(port.c_str()));
}


std::string SessionManager::getSelectSessionName() {
    int row = sessionListView->currentRow();
    QTableWidgetItem *titleItem = sessionListView->item(row, 0);
    QString title = titleItem->text();
    return title.toStdString();
}

void SessionManager::onActionEditSession() {
    std::string sessionName = getSelectSessionName();
    std::string sessionType = ConfigManager::getInstance()->getString("sessions", sessionName.c_str(), "unknown");
    EditSessionDialog *dialog = new EditSessionDialog(sessionName, sessionType, this);
    dialog->show();
}

void SessionManager::onActionDeleteSession() {

}

