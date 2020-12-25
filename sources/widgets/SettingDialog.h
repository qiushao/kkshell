//
// Created by qiushao on 2020/12/10.
//

#ifndef KKSHELL_SETTINGDIALOG_H
#define KKSHELL_SETTINGDIALOG_H

#include <QFormLayout>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QFontComboBox>
#include <QVBoxLayout>

#include "setting/Settings.h"

class SettingDialog : public QDialog  {
Q_OBJECT
public:
    SettingDialog(QWidget *parent);
    ~SettingDialog() override;


private:
    void initWidgets();
    void fillColorScheme();
    void onOK();
    void onCancel();

    QVBoxLayout *_mainLayout = nullptr;
    QFormLayout *_formLayout = nullptr;

    QFontComboBox *_fontFamilyEdit = nullptr;
    QLineEdit *_fontSizeEdit = nullptr;
    QComboBox *_colorSchemeEdit = nullptr;

    QHBoxLayout *_buttonLayout = nullptr;
    QPushButton *_okButton = nullptr;
    QPushButton *_cancelButton = nullptr;
};


#endif //KKSHELL_SETTINGDIALOG_H
