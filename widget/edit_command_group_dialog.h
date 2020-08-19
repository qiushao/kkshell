//
// Created by min on 2020/8/17.
//

#ifndef KKSHELL_EDIT_COMMAND_GROUP_DIALOG_H
#define KKSHELL_EDIT_COMMAND_GROUP_DIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class EditCommandGroupDialog : public QDialog {

Q_OBJECT

public:
    explicit EditCommandGroupDialog(QWidget *parent);
    ~EditCommandGroupDialog() override;

    void clearField();
    void setField(const std::string &groupName);

signals:
    void commandGroupChanged(const QString &groupName);
    void commandGroupAdd(const QString &groupName);

protected:
    bool isEdit = false;
    std::string oldGroupName = "";

    void onApply();
    void onCancel();

    QVBoxLayout *mainLayout;
    QHBoxLayout *titleLayout;
    QLabel *titleLabel;
    QLineEdit *titleEdit;

    QHBoxLayout *actionLayout;
    QPushButton *applyButton;
    QPushButton *cancelButton;
};


#endif //KKSHELL_EDIT_COMMAND_GROUP_DIALOG_H
