//
// Created by min on 2020/8/10.
//

#include "settings.h"
#include <QDebug>
#include <qtermwidget5/qtermwidget.h>
#include "resources/forms/ui_settings.h"
#include "common/config/config_manager.h"

Settings::Settings(QWidget *parent) : QDialog(parent), ui(new Ui::Settings) {
    ui->setupUi(this);

    ConfigManager *conf = ConfigManager::getInstance();
    QString fontFamily = conf->getCString("app", "fontFamily", "Monospace");
    int fontSize = conf->getInt("app", "fontSize", 18);
    QString scheme = conf->getCString("app", "colorScheme", "Tango");

    fillColorScheme();

    ui->fontComboBox->setCurrentText(fontFamily);
    ui->fontSizeEdit->setText(QString::number(fontSize));
    ui->colorSchemeComboBox->setCurrentText(scheme);

    QObject::connect(ui->applyButton, &QPushButton::clicked, this, &Settings::updateSettings);
}

Settings::~Settings() {

}

void Settings::updateSettings() {
    hide();
    QString fontFamily = ui->fontComboBox->currentText();
    int fontSize = ui->fontSizeEdit->text().toInt();
    QString scheme = ui->colorSchemeComboBox->currentText();

    ConfigManager *conf = ConfigManager::getInstance();
    conf->setCString("app", "fontFamily", fontFamily.toStdString().c_str());
    conf->setInt("app", "fontSize", fontSize);
    conf->setCString("app", "colorScheme", scheme.toStdString().c_str());

}

void Settings::fillColorScheme() {
    QStringList schemes = QTermWidget::availableColorSchemes();
    qDebug() << "scheme = " << schemes;
    for (QString scheme : schemes) {
        ui->colorSchemeComboBox->addItem(scheme, scheme);
    }
}
