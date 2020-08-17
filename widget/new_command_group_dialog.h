//
// Created by min on 2020/8/17.
//

#ifndef KKSHELL_NEW_COMMAND_GROUP_DIALOG_H
#define KKSHELL_NEW_COMMAND_GROUP_DIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class NewCommandGroupDialog : public QDialog {
public:
    explicit NewCommandGroupDialog(QWidget *parent);
    ~NewCommandGroupDialog() override;

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


#endif //KKSHELL_NEW_COMMAND_GROUP_DIALOG_H
