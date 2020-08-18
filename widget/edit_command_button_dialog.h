//
// Created by min on 2020/8/17.
//

#ifndef KKSHELL_EDIT_COMMAND_BUTTON_DIALOG_H
#define KKSHELL_EDIT_COMMAND_BUTTON_DIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class EditCommandButtonDialog : public QDialog {

Q_OBJECT

public:
    explicit EditCommandButtonDialog(QWidget *parent);
    ~EditCommandButtonDialog() override;

    void clearField();
    void setField(const std::string &groupName, const std::string &commandName, const std::string &command);

    signals:
    void commandButtonChanged(const QString &groupName);

protected:
    bool isEdit = false;
    std::string groupName_ = "";
    std::string oldCommandName = "";

    void onApply();
    void onCancel();

    QVBoxLayout *mainLayout = nullptr;
    QHBoxLayout *titleLayout = nullptr;
    QLabel *titleLabel = nullptr;
    QLineEdit *titleEdit = nullptr;

    QHBoxLayout *commandLayout = nullptr;
    QLabel *commandLabel = nullptr;
    QLineEdit *commandEdit = nullptr;

    QHBoxLayout *actionLayout = nullptr;
    QPushButton *applyButton = nullptr;
    QPushButton *cancelButton = nullptr;
};


#endif //KKSHELL_EDIT_COMMAND_BUTTON_DIALOG_H
