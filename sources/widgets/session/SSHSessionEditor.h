//
// Created by qiushao on 2020/12/5.
//

#ifndef KKSHELL_SSHSESSIONEDITOR_H
#define KKSHELL_SSHSESSIONEDITOR_H

#include <QFormLayout>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>

#include "setting/Settings.h"

class SSHSessionEditor : public QDialog {
Q_OBJECT
public:
    SSHSessionEditor(QWidget *parent, const QString &categoryName);
    SSHSessionEditor(QWidget *parent, const QString &categoryName, const Session &session);
    ~SSHSessionEditor() override;

signals:
    void newSession(const QString &categoryName, const Session &session);
    void updateSession(const QString &categoryName, const Session &oldSession, const Session &newSession);

private:
    void initWidgets();
    void onOK();
    void onCancel();

    bool _isEdit = false;
    QString _categoryName;
    Session _oldSession;

    QVBoxLayout *_mainLayout = nullptr;

    QFormLayout *_formLayout = nullptr;
    QLineEdit *_categoryEdit = nullptr;
    QLineEdit *_sessionNameEdit = nullptr;

    QLineEdit *_hostEdit = nullptr;
    QLineEdit *_portEdit = nullptr;
    QLineEdit *_userNameEdit = nullptr;
    QComboBox *_authTypeEdit = nullptr;
    QLineEdit *_passwdEdit = nullptr;
    QLineEdit *_keyFileEdit = nullptr;

    QHBoxLayout *_buttonLayout = nullptr;
    QPushButton *_okButton = nullptr;
    QPushButton *_cancelButton = nullptr;
};


#endif //KKSHELL_SSHSESSIONEDITOR_H
