//
// Created by qiushao on 2020/12/12.
//

#ifndef KKSHELL_COMMANDBUTTONEDITOR_H
#define KKSHELL_COMMANDBUTTONEDITOR_H

#include <QDialog>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "setting/Settings.h"

class CommandButtonEditor : public QDialog {
Q_OBJECT

public:
    explicit CommandButtonEditor(QWidget *parent, const QString &categoryName);
    CommandButtonEditor(QWidget *parent, const QString &categoryName, const Command &command);
    ~CommandButtonEditor() override;

signals:
    void reloadCommandCategory(const QString &categoryName);
private:
    void initWidgets();
    void onOK();
    void onCancel();

    bool _isEdit = false;
    QString _categoryName;
    Command _oldCommand;

    QVBoxLayout *_mainLayout = nullptr;

    QFormLayout *_formLayout = nullptr;
    QLineEdit *_categoryEdit = nullptr;
    QLineEdit *_commandNameEdit = nullptr;
    QLineEdit *_commandStringEdit = nullptr;

    QHBoxLayout *_buttonLayout = nullptr;
    QPushButton *_okButton = nullptr;
    QPushButton *_cancelButton = nullptr;
};


#endif //KKSHELL_COMMANDBUTTONEDITOR_H
