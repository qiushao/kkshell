//
// Created by mint on 2020/8/12.
//

#ifndef KKSHELL_EDIT_SESSION_DIALOG_H
#define KKSHELL_EDIT_SESSION_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>

class EditSessionDialog : public QDialog {

Q_OBJECT

public:
    explicit EditSessionDialog(const std::string &sessionType, QWidget *parent);
    ~EditSessionDialog() override;

    void editSession(const std::string &sessionName, const std::string &sessionType);
    void editSerialSession(const std::string &sessionName);
    void editSSHSession(const std::string &sessionName);

    signals:
    void sessionListUpdate();

protected:
    void initViews(const std::string &sessionType);
    void newSerialSessionLayout();
    void newSSHSessionLayout();
    void newButtonLayout();
    void onSSHAuthTypeChanged(const QString &authType);

    virtual void onApplyButtonClicked();
    virtual void onCancelButtonClicked();
    void saveSession();

    bool isEdit = false;
    std::string oldSessionName = "";

    QVBoxLayout *mainLayout = nullptr;
    QHBoxLayout *sessionTypeLayout = nullptr;
    QLabel *sessionTypeLabel = nullptr;
    QComboBox *sessionTypeComboBox = nullptr;

    QHBoxLayout *sessionNameLayout = nullptr;
    QLabel *sessionNameLabel = nullptr;
    QLineEdit *sessionNameEidt = nullptr;

    QVBoxLayout *serialSessionLayout = nullptr;
    QHBoxLayout *serialDevLayout = nullptr;
    QLabel *serialDevLabel = nullptr;
    QComboBox *serialDevComboBox = nullptr;
    QHBoxLayout *serialBaudRateLayout = nullptr;
    QLabel *serialBaudRateLabel = nullptr;
    QComboBox *serialBaudRateComboBox = nullptr;
    QHBoxLayout *serialDataBitsLayout = nullptr;
    QLabel *serialDataBitsLabel = nullptr;
    QComboBox *serialDataBitsComboBox = nullptr;
    QHBoxLayout *serialStopBitsLayout = nullptr;
    QLabel *serialStopBitsLabel = nullptr;
    QComboBox *serialStopBitsComboBox = nullptr;
    QHBoxLayout *serialParityLayout = nullptr;
    QLabel *serialParityLabel = nullptr;
    QComboBox *serialParityComboBox = nullptr;
    QHBoxLayout *serialFlowControlLayout = nullptr;
    QLabel *serialFlowControlLabel = nullptr;
    QComboBox *serialFlowControlComboBox = nullptr;

    QVBoxLayout *sshSessionLayout = nullptr;
    QHBoxLayout *sshHostLayout = nullptr;
    QLabel *sshHostLabel = nullptr;
    QLineEdit *sshHostEdit = nullptr;
    QHBoxLayout *sshPortLayout = nullptr;
    QLabel *sshPortLabel = nullptr;
    QLineEdit *sshPortEdit = nullptr;
    QHBoxLayout *sshAuthTypeLayout = nullptr;
    QLabel *sshAuthTypeLabel = nullptr;
    QComboBox *sshAuthTypeEdit = nullptr;
    QHBoxLayout *sshUserLayout = nullptr;
    QLabel *sshUserLabel = nullptr;
    QLineEdit *sshUserEdit = nullptr;
    QHBoxLayout *sshPasswdLayout = nullptr;
    QLabel *sshPasswdLabel = nullptr;
    QLineEdit *sshPasswdEdit = nullptr;
    QHBoxLayout *sshKeyFileLayout = nullptr;
    QLabel *sshKeyFileLabel = nullptr;
    QLineEdit *sshKeyFileEdit = nullptr;

    QHBoxLayout *buttonLayout = nullptr;
    QPushButton *cancelButton = nullptr;
    QPushButton *applyButton = nullptr;
};


#endif //KKSHELL_EDIT_SESSION_DIALOG_H
