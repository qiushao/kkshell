//
// Created by qiushao on 2020/11/29.
//

#include "SessionCategoryEditor.h"
#include "setting/SettingManager.h"

SessionCategoryEditor::SessionCategoryEditor(QWidget *parent) : QDialog(parent) {
    initWidgets();
}

SessionCategoryEditor::SessionCategoryEditor(QWidget *parent, const QString &oldCategoryName) {
    initWidgets();
    _categoryNameEdit->setText(oldCategoryName);
    _oldCategoryName = oldCategoryName;
    _isEdit = true;
}

SessionCategoryEditor::~SessionCategoryEditor() {

}

void SessionCategoryEditor::initWidgets() {
    _mainLayout = new QVBoxLayout(this);
    setLayout(_mainLayout);

    _formLayout = new QFormLayout(this);
    _mainLayout->addLayout(_formLayout);

    _categoryNameEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Category："), _categoryNameEdit);

    _buttonLayout = new QHBoxLayout(this);
    _okButton = new QPushButton(tr("ok"));
    _cancelButton = new QPushButton(tr("cancel"));
    _buttonLayout->addWidget(_cancelButton);
    _buttonLayout->addWidget(_okButton);
    _mainLayout->addLayout(_buttonLayout);

    QObject::connect(_okButton, &QPushButton::clicked, this, &SessionCategoryEditor::onOK);
    QObject::connect(_cancelButton, &QPushButton::clicked, this, &SessionCategoryEditor::onCancel);
}

void SessionCategoryEditor::onOK() {
    SettingManager *settingManager = SettingManager::getInstance();
    QString categoryName = _categoryNameEdit->text();
    if (_isEdit) {
        if (categoryName != _oldCategoryName) {
            settingManager->renameSessionCategory(_oldCategoryName.toStdString(), categoryName.toStdString());
            emit renameSessionCategory(_oldCategoryName, categoryName);
        }
    } else {
        settingManager->addSessionCategory(categoryName.toStdString());
        SessionCategory category;
        category.categoryName = categoryName.toStdString();
        emit newSessionCategory(category);
    }

    close();
}

void SessionCategoryEditor::onCancel() {
    close();
}


