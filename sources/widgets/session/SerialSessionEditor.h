//
// Created by qiushao on 2020/12/5.
//

#ifndef KKSHELL_SERIALSESSIONEDITOR_H
#define KKSHELL_SERIALSESSIONEDITOR_H

#include <QFormLayout>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>

#include "setting/Settings.h"

class SerialSessionEditor : public QDialog {
Q_OBJECT
public:
    SerialSessionEditor(QWidget *parent, const QString &categoryName);
    SerialSessionEditor(QWidget *parent, const QString &categoryName, const Session &session);
    ~SerialSessionEditor() override;

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

    QComboBox *_devEdit = nullptr;
    QLineEdit *_baudRateEdit = nullptr;
    QLineEdit *_dataBitsEdit = nullptr;
    QLineEdit *_stopBitsEdit = nullptr;
    QLineEdit *_parityEdit = nullptr;
    QLineEdit *_flowControlEdit = nullptr;

    QHBoxLayout *_buttonLayout = nullptr;
    QPushButton *_okButton = nullptr;
    QPushButton *_cancelButton = nullptr;
};


#endif //KKSHELL_SERIALSESSIONEDITOR_H
