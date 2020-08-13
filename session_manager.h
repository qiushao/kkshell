//
// Created by mint on 2020/8/10.
//

#ifndef KKSHELL_SESSION_MANAGER_H
#define KKSHELL_SESSION_MANAGER_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class SessionManager; }
QT_END_NAMESPACE

class SessionManager : public QMainWindow {
Q_OBJECT

public:
    explicit SessionManager(QWidget *parent = nullptr);
    ~SessionManager() override;
    void updateSessionList();
    void onItemDoubleClicked(QTableWidgetItem *item);

signals:
    void openSession(std::string session);

private:
    void onActionEditSession();

    void onActionDeleteSession();
    void fillSessionList();

    void addLocalShellSessionToList(std::string session, int row);
    void addSerialSessionToList(std::string session, int row);
    void addSSHSessionToList(std::string session, int row);
    std::string getSelectSessionName();

    Ui::SessionManager *ui = nullptr;
    QTableWidget *sessionListView;
};


#endif //KKSHELL_SESSION_MANAGER_H
