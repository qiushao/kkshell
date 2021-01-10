//
// Created by qiushao on 2021/1/6.
//

#include <QDebug>
#include <QTabBar>
#include "SessionTabWidget.h"
#include "TableTitleEditor.h"

SessionTabWidget::SessionTabWidget(QWidget *parent) : QTabWidget(parent) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    _menu = new QMenu(this);
    _renameTableAction = new QAction(tr("Rename Table"));
    connect(_renameTableAction, &QAction::triggered, this, &SessionTabWidget::onRenameTableAction);
    _menu->addAction(_renameTableAction);
    _menu->addSeparator();

    _closeTableAction = new QAction(tr("Close Table"));
    connect(_closeTableAction, &QAction::triggered, this, &SessionTabWidget::onCloseTableAction);
    _menu->addAction(_closeTableAction);

    _closeOtherTableAction = new QAction(tr("Close Other Table"));
    connect(_closeOtherTableAction, &QAction::triggered, this, &SessionTabWidget::onCloseOtherTableAction);
//    _menu->addAction(_closeOtherTableAction);

    _closeAllTableAction = new QAction(tr("Close All Table"));
    connect(_closeAllTableAction, &QAction::triggered, this, &SessionTabWidget::onCloseAllTableAction);
//    _menu->addAction(_closeAllTableAction);

    connect(this, &SessionTabWidget::customContextMenuRequested, this, &SessionTabWidget::onPopMenu);
}

SessionTabWidget::~SessionTabWidget() {

}

void SessionTabWidget::onPopMenu(const QPoint &point) {
    _tabIndex = tabBar()->tabAt(point);
    if (_tabIndex >= 0) {
        _menu->exec(QCursor::pos());
    }
}

void SessionTabWidget::onRenameTableAction() {
    TableTitleEditor titleEditor(this, this->tabText(_tabIndex));
    connect(&titleEditor, &TableTitleEditor::updateTableTitle, this, &SessionTabWidget::updateTableTitle);
    titleEditor.exec();
}

void SessionTabWidget::onCloseTableAction() {
    this->removeTab(_tabIndex);
}

void SessionTabWidget::onCloseOtherTableAction() {
    while (this->count() > _tabIndex+1) {
        this->removeTab(_tabIndex+1);
    }

    while (this->count() > 1) {
        this->removeTab(0);
    }
}

void SessionTabWidget::onCloseAllTableAction() {
    while (this->count() > 0) {
        this->removeTab(0);
    }
}

void SessionTabWidget::updateTableTitle(const QString &newTitle) {
    this->setTabText(_tabIndex, newTitle);
}
