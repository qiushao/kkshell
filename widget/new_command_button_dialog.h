//
// Created by min on 2020/8/17.
//

#ifndef KKSHELL_NEW_COMMAND_BUTTON_DIALOG_H
#define KKSHELL_NEW_COMMAND_BUTTON_DIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class NewCommandButtonDialog : public QDialog {

Q_OBJECT

public:
    explicit NewCommandButtonDialog(QWidget *parent);
    ~NewCommandButtonDialog() override;

    void reset(const std::string &groupName);

    signals:
    void commandButtonChanged(const QString &groupName);

protected:
    bool isEdit = false;
    std::string groupName_ = "";
    std::string oldCommandName = "";

    void onApply();
    void onCancel();

    QVBoxLayout *mainLayout;
    QHBoxLayout *titleLayout;
    QLabel *titleLabel;
    QLineEdit *titleEdit;

    QHBoxLayout *commandLayout;
    QLabel *commandLabel;
    QLineEdit *commandEdit;

    QHBoxLayout *actionLayout;
    QPushButton *applyButton;
    QPushButton *cancelButton;
};


#endif //KKSHELL_NEW_COMMAND_BUTTON_DIALOG_H
