//
// Created by qiushao on 2020/12/12.
//

#ifndef KKSHELL_COMMANDCATEGORYEDITOR_H
#define KKSHELL_COMMANDCATEGORYEDITOR_H

#include <QDialog>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include "setting/Settings.h"

class CommandCategoryEditor : public QDialog {
Q_OBJECT

public:
    explicit CommandCategoryEditor(QWidget *parent);
    CommandCategoryEditor(QWidget *parent, const QString &oldCategoryName);
    ~CommandCategoryEditor() override;

signals:
    void newCommandCategory(const CommandCategory &category);
    void renameCommandCategory(const QString &oldCategoryName, const QString &newCategoryName);

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


#endif //KKSHELL_COMMANDCATEGORYEDITOR_H
