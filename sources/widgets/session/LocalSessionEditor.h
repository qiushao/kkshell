//
// Created by qiushao on 2020/12/5.
//

#ifndef KKSHELL_LOCALSESSIONEDITOR_H
#define KKSHELL_LOCALSESSIONEDITOR_H

#include <QDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "setting/Settings.h"

class LocalSessionEditor : public QDialog {
    Q_OBJECT
public:
    LocalSessionEditor(QWidget *parent, const QString &categoryName);
    LocalSessionEditor(QWidget *parent, const QString &categoryName, const Session &session);
    ~LocalSessionEditor() override;

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

    QHBoxLayout *_buttonLayout = nullptr;
    QPushButton *_okButton = nullptr;
    QPushButton *_cancelButton = nullptr;
};


#endif //KKSHELL_LOCALSESSIONEDITOR_H
