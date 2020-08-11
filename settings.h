//
// Created by min on 2020/8/10.
//

#ifndef KKSHELL_SETTINGS_H
#define KKSHELL_SETTINGS_H


#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Settings; }
QT_END_NAMESPACE

class Settings : public QDialog {

Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings() override;

    void updateSettings();

private:
    void fillColorScheme();
    Ui::Settings *ui;
};


#endif //KKSHELL_SETTINGS_H
