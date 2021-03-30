//
// Created by qiushao on 2020/11/29.
//

#include <QEvent>
#include <QKeyEvent>
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
    _titleEdit->installEventFilter(this);
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

bool TableTitleEditor::eventFilter(QObject *obj, QEvent *e) {
    if (e->type() == QEvent::KeyPress) {
        auto *event = dynamic_cast<QKeyEvent *>(e);
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            onOK();
            return true;
        }
    }
    return false;
}

void TableTitleEditor::onOK() {
    emit updateTableTitle(_titleEdit->text());
    close();
}

void TableTitleEditor::onCancel() {
    close();
}


