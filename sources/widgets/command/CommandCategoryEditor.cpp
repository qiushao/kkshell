//
// Created by qiushao on 2020/12/12.
//

#include "CommandCategoryEditor.h"
#include "setting/SettingManager.h"

CommandCategoryEditor::CommandCategoryEditor(QWidget *parent) : QDialog(parent) {
    initWidgets();
}

CommandCategoryEditor::CommandCategoryEditor(QWidget *parent, const QString &oldCategoryName) {
    initWidgets();
    _categoryNameEdit->setText(oldCategoryName);
    _oldCategoryName = oldCategoryName;
    _isEdit = true;
}

CommandCategoryEditor::~CommandCategoryEditor() {

}

void CommandCategoryEditor::initWidgets() {
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

    QObject::connect(_okButton, &QPushButton::clicked, this, &CommandCategoryEditor::onOK);
    QObject::connect(_cancelButton, &QPushButton::clicked, this, &CommandCategoryEditor::onCancel);
}

void CommandCategoryEditor::onOK() {
    SettingManager *settingManager = SettingManager::getInstance();
    QString categoryName = _categoryNameEdit->text();
    if (_isEdit) {
        if (categoryName != _oldCategoryName) {
            settingManager->renameCommandCategory(_oldCategoryName.toStdString(), categoryName.toStdString());
            emit renameCommandCategory(_oldCategoryName, categoryName);
        }
    } else {
        settingManager->addCommandCategory(categoryName.toStdString());
        CommandCategory category;
        category.categoryName = categoryName.toStdString();
        emit newCommandCategory(category);
    }

    close();
}

void CommandCategoryEditor::onCancel() {
    close();
}


