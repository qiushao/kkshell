//
// Created by qiushao on 2020/12/10.
//

#include "SettingDialog.h"
#include "setting/SettingManager.h"
#include "qtermwidget.h"

SettingDialog::SettingDialog(QWidget *parent) :QDialog(parent) {
    initWidgets();
    fillColorScheme();
    auto settingManager = SettingManager::getInstance();
    _fontFamilyEdit->setCurrentText(settingManager->getFontFamily().c_str());
    _fontSizeEdit->setText(QString::number(settingManager->getFontSize()));
    _colorSchemeEdit->setCurrentText(settingManager->getColorScheme().c_str());
}

SettingDialog::~SettingDialog() {

}

void SettingDialog::initWidgets() {
    _mainLayout = new QVBoxLayout(this);
    setLayout(_mainLayout);

    _formLayout = new QFormLayout(this);
    _mainLayout->addLayout(_formLayout);

    _fontFamilyEdit = new QFontComboBox(this);
    _formLayout->addRow(tr("Font Family:"), _fontFamilyEdit);

    _fontSizeEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Font Size:"), _fontSizeEdit);

    _colorSchemeEdit = new QComboBox(this);
    _formLayout->addRow(tr("Color Scheme:"), _colorSchemeEdit);

    _buttonLayout = new QHBoxLayout(this);
    _mainLayout->addLayout(_buttonLayout);

    _okButton = new QPushButton(tr("OK"));
    _cancelButton = new QPushButton(tr("Cancel"));
    _buttonLayout->addWidget(_cancelButton);
    _buttonLayout->addWidget(_okButton);

    QObject::connect(_okButton, &QPushButton::clicked, this, &SettingDialog::onOK);
    QObject::connect(_cancelButton, &QPushButton::clicked, this, &SettingDialog::onCancel);
}

void SettingDialog::fillColorScheme() {
    QStringList schemes = QTermWidget::availableColorSchemes();
    for (const QString &scheme : schemes) {
        _colorSchemeEdit->addItem(scheme, scheme);
    }
}

void SettingDialog::onOK() {
    SettingManager *settingManager = SettingManager::getInstance();
    settingManager->setFontFamily(_fontFamilyEdit->currentText().toStdString());
    settingManager->setFontSize(_fontSizeEdit->text().toInt());
    settingManager->setColorScheme(_colorSchemeEdit->currentText().toStdString());
    close();
}

void SettingDialog::onCancel() {
    close();
}
