//
// Created by qiushao on 2020/11/29.
//

#include "TableTitleEditor.h"
#include "setting/SettingManager.h"

TableTitleEditor::TableTitleEditor(QWidget *parent, const QString &oldCategoryName) {
    initWidgets();
    _titleEdit->setText(oldCategoryName);
}

TableTitleEditor::~TableTitleEditor() {

}

void TableTitleEditor::initWidgets() {
    _mainLayout = new QVBoxLayout(this);
    setLayout(_mainLayout);

    _formLayout = new QFormLayout(this);
    _mainLayout->addLayout(_formLayout);

    _titleEdit = new QLineEdit(this);
    _formLayout->addRow(tr("Tab Title："), _titleEdit);

    _buttonLayout = new QHBoxLayout(this);
    _okButton = new QPushButton(tr("ok"));
    _cancelButton = new QPushButton(tr("cancel"));
    _buttonLayout->addWidget(_cancelButton);
    _buttonLayout->addWidget(_okButton);
    _mainLayout->addLayout(_buttonLayout);

    QObject::connect(_okButton, &QPushButton::clicked, this, &TableTitleEditor::onOK);
    QObject::connect(_cancelButton, &QPushButton::clicked, this, &TableTitleEditor::onCancel);
}

void TableTitleEditor::onOK() {
    emit updateTableTitle(_titleEdit->text());
    close();
}

void TableTitleEditor::onCancel() {
    close();
}


