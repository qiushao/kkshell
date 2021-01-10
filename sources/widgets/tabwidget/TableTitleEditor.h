//
// Created by qiushao on 2020/11/29.
//

#ifndef KKSHELL_TABLETITLEEDITOR_H
#define KKSHELL_TABLETITLEEDITOR_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>

class TableTitleEditor : public QDialog {
Q_OBJECT

public:
    TableTitleEditor(QWidget *parent, const QString &oldTitleName);
    ~TableTitleEditor() override;

signals:
    void updateTableTitle(const QString &newTitle);

private:
    void initWidgets();
    void onOK();
    void onCancel();

    QVBoxLayout *_mainLayout = nullptr;

    QFormLayout *_formLayout = nullptr;
    QLineEdit *_titleEdit = nullptr;

    QHBoxLayout *_buttonLayout = nullptr;
    QPushButton *_okButton = nullptr;
    QPushButton *_cancelButton = nullptr;
};


#endif //KKSHELL_TABLETITLEEDITOR_H
