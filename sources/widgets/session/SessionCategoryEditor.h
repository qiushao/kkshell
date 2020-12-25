//
// Created by qiushao on 2020/11/29.
//

#ifndef KKSHELL_SESSIONCATEGORYEDITOR_H
#define KKSHELL_SESSIONCATEGORYEDITOR_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

#include "setting/Settings.h"

class SessionCategoryEditor : public QDialog {
Q_OBJECT

public:
    explicit SessionCategoryEditor(QWidget *parent);
    SessionCategoryEditor(QWidget *parent, const QString &oldCategoryName);
    ~SessionCategoryEditor() override;

signals:
    void newSessionCategory(const SessionCategory &category);
    void renameSessionCategory(const QString &oldCategoryName, const QString &newCategoryName);

private:
    void initWidgets();
    void onOK();
    void onCancel();

    bool _isEdit = false;
    QString _oldCategoryName = "";

    QVBoxLayout *_mainLayout = nullptr;

    QFormLayout *_formLayout = nullptr;
    QLineEdit *_categoryNameEdit = nullptr;

    QHBoxLayout *_buttonLayout = nullptr;
    QPushButton *_okButton = nullptr;
    QPushButton *_cancelButton = nullptr;
};


#endif //KKSHELL_SESSIONCATEGORYEDITOR_H
