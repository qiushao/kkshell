//
// Created by qiushao on 2020/11/1.
//

#include "SessionListWidget.h"
#include <QIcon>
#include <QDebug>
#include <setting/SettingManager.h>

#include "widgets/session/SessionCategoryEditor.h"
#include "widgets/session/LocalSessionEditor.h"
#include "widgets/session/SSHSessionEditor.h"
#include "widgets/session/SerialSessionEditor.h"

SessionListWidget::SessionListWidget(QWidget *parent) : QTreeWidget(parent) {
    setHeaderHidden(true);
    setContextMenuPolicy(Qt::CustomContextMenu);

    initIcon();
    initAction();
    initPopMenu();
}

SessionListWidget::~SessionListWidget() {

}

void SessionListWidget::initIcon() {
    _categoryIcon = new QIcon(":/images/category.png");
    _sessionIcon = new QIcon(":/images/session.png");
}

void SessionListWidget::initAction() {
    _newCategoryAction = new QAction(tr("New Category"), this);
    connect(_newCategoryAction, &QAction::triggered, this, &SessionListWidget::onNewCategoryAction);

    _renameCategoryAction = new QAction(tr("Rename Category"), this);
    connect(_renameCategoryAction, &QAction::triggered, this, &SessionListWidget::onRenameCategoryAction);

    _removeCategoryAction = new QAction(tr("Remove Category"), this);
    connect(_removeCategoryAction, &QAction::triggered, this, &SessionListWidget::onRemoveCategoryAction);

    _moveUpCategoryAction = new QAction(tr("Move Up Category"), this);
    connect(_moveUpCategoryAction, &QAction::triggered, this, &SessionListWidget::onMoveUpCategoryAction);

    _moveDownCategoryAction = new QAction(tr("Move Down Category"), this);
    connect(_moveDownCategoryAction, &QAction::triggered, this, &SessionListWidget::onMoveDownCategoryAction);

    _newLocalSessionAction = new QAction(tr("New Local Session"), this);
    connect(_newLocalSessionAction, &QAction::triggered, this, &SessionListWidget::onNewLocalSessionAction);

    _newSerialSessionAction = new QAction(tr("New Serial Session"), this);
    connect(_newSerialSessionAction, &QAction::triggered, this, &SessionListWidget::onNewSerialSessionAction);

    _newSSHSessionAction = new QAction(tr("New SSH Session"), this);
    connect(_newSSHSessionAction, &QAction::triggered, this, &SessionListWidget::onNewSSHSessionAction);

    _editSessionAction = new QAction(tr("Edit Session"), this);
    connect(_editSessionAction, &QAction::triggered, this, &SessionListWidget::onEditSessionActioin);

    _removeSessionAction = new QAction(tr("Remove Session"), this);
    connect(_removeSessionAction, &QAction::triggered, this, &SessionListWidget::onRemoveSessionAction);

    _moveUpSessionAction = new QAction(tr("Move Up Session"), this);
    connect(_moveUpSessionAction, &QAction::triggered, this, &SessionListWidget::onMoveUpSessionAction);

    _moveDownSessionAction = new QAction(tr("Move Down Session"), this);
    connect(_moveDownSessionAction, &QAction::triggered, this, &SessionListWidget::onMoveDownSessionAction);
}

void SessionListWidget::initPopMenu() {
    _emptyAreaMenu = new QMenu(this);
    _emptyAreaMenu->addAction(_newCategoryAction);

    _categoryMenu = new QMenu(this);
    _categoryMenu->addAction(_newCategoryAction);
    _categoryMenu->addAction(_renameCategoryAction);
    _categoryMenu->addAction(_removeCategoryAction);
    _categoryMenu->addAction(_moveUpCategoryAction);
    _categoryMenu->addAction(_moveDownCategoryAction);

    _categoryMenu->addSeparator();
    _categoryMenu->addAction(_newLocalSessionAction);
    _categoryMenu->addAction(_newSSHSessionAction);
    _categoryMenu->addAction(_newSerialSessionAction);

    _sessionMenu = new QMenu(this);
    _sessionMenu->addAction(_editSessionAction);
    _sessionMenu->addAction(_removeSessionAction);
    _sessionMenu->addAction(_moveUpSessionAction);
    _sessionMenu->addAction(_moveDownSessionAction);
}

void SessionListWidget::loadSessionList() {
    const std::vector<SessionCategory> &sessionCategory = SettingManager::getInstance()->getSessionCategoryList();
    for (const SessionCategory &category : sessionCategory) {
        auto *categoryItem = new QTreeWidgetItem(this, QStringList(category.categoryName.c_str()));
        QVariant categoryVar = QVariant::fromValue(category);
        categoryItem->setData(0, Qt::UserRole, categoryVar);
        categoryItem->setIcon(0, *_categoryIcon);
        for (const Session &session : category.sessionList) {
            auto *sessionItem = new QTreeWidgetItem(categoryItem, QStringList(session.sessionName.c_str()));
            QVariant sessionVar = QVariant::fromValue(session);
            sessionItem->setData(0, Qt::UserRole, sessionVar);
            sessionItem->setIcon(0, *_sessionIcon);
            categoryItem->addChild(sessionItem);
        }
        addTopLevelItem(categoryItem);
    }

    connect(this, &QTreeWidget::itemDoubleClicked, this, &SessionListWidget::onItemDoubleClicked);
    connect(this, &QTreeWidget::customContextMenuRequested, this, &SessionListWidget::onPopMenu);
}

void SessionListWidget::onItemDoubleClicked(QTreeWidgetItem *item, int column) {
    QVariant var = item->data(column, Qt::UserRole);
    if (var.canConvert<Session>()) {
        Session session = var.value<Session>();
        emit requestOpenSession(session);
    }
}

void SessionListWidget::onPopMenu(const QPoint &point) {
    QTreeWidgetItem *curItem = itemAt(point);
    if (curItem == nullptr) {
        clearSelection();
        _emptyAreaMenu->exec(QCursor::pos());
    } else if (curItem->parent() == nullptr) {
        _categoryMenu->exec(QCursor::pos());
    } else {
        _sessionMenu->exec(QCursor::pos());
    }
}

void SessionListWidget::onNewCategoryAction() {
    SessionCategoryEditor categoryEditor(this);
    connect(&categoryEditor, &SessionCategoryEditor::newSessionCategory, this,
            &SessionListWidget::onNewSessionCategory);
    categoryEditor.exec();
}

void SessionListWidget::onRenameCategoryAction() {
    SessionCategoryEditor categoryEditor(this, currentItem()->text(0));
    connect(&categoryEditor, &SessionCategoryEditor::renameSessionCategory, this,
            &SessionListWidget::onRenameSessionCategory);
    categoryEditor.exec();
}

void SessionListWidget::onNewSessionCategory(const SessionCategory &category) {
    auto *categoryItem = new QTreeWidgetItem(this, QStringList(category.categoryName.c_str()));
    categoryItem->setIcon(0, *_categoryIcon);
    QVariant categoryVar = QVariant::fromValue(category);
    categoryItem->setData(0, Qt::UserRole, categoryVar);
    addTopLevelItem(categoryItem);
}

void SessionListWidget::onRenameSessionCategory(const QString &oldCategoryName, const QString &newCategoryName) {
    currentItem()->setText(0, newCategoryName);
}

void SessionListWidget::onRemoveCategoryAction() {
    bool ret = SettingManager::getInstance()->deleteSessionCategory(currentItem()->text(0).toStdString());
    if (!ret) {
        return;
    }

    QTreeWidgetItem *item = currentItem();
    if (item->parent() == nullptr) {
        delete takeTopLevelItem(currentIndex().row());
    } else {
        delete item->parent()->takeChild(currentIndex().row());
    }
}

void SessionListWidget::onMoveUpCategoryAction() {
    qDebug() << "onMoveUpCategoryAction";
    QTreeWidgetItem *item = this->currentItem();
    if (item == topLevelItem(0)) {
        qDebug() << "item already in top";
        return;
    }

    for (int i = 0; i < topLevelItemCount(); ++i) {
        if (item == topLevelItem(i)) {
            qDebug() << "found item at " << i;
            item = takeTopLevelItem(i);
            insertTopLevelItem(i-1, item);
            break;
        }
    }

    SettingManager::getInstance()->moveUpSessionCategory(item->text(0).toStdString());
}

void SessionListWidget::onMoveDownCategoryAction() {
    qDebug() << "onMoveDownCategoryAction";
    QTreeWidgetItem *item = this->currentItem();
    if (item == topLevelItem(topLevelItemCount()-1)) {
        qDebug() << "item already in bottom";
        return;
    }

    for (int i = 0; i < topLevelItemCount(); ++i) {
        if (item == topLevelItem(i)) {
            qDebug() << "found item at " << i;
            item = takeTopLevelItem(i);
            insertTopLevelItem(i+1, item);
            break;
        }
    }
    SettingManager::getInstance()->moveDownSessionCategory(item->text(0).toStdString());
}

void SessionListWidget::onNewLocalSessionAction() {
    qDebug() << "onNewLocalSessionAction";
    LocalSessionEditor localSessionEditor(this, currentItem()->text(0));
    connect(&localSessionEditor, &LocalSessionEditor::newSession, this, &SessionListWidget::onNewSession);
    localSessionEditor.exec();
}

void SessionListWidget::onNewSerialSessionAction() {
    qDebug() << "onNewSerialSessionAction";
    SerialSessionEditor serialSessionEditor(this, currentItem()->text(0));
    connect(&serialSessionEditor, &SerialSessionEditor::newSession, this, &SessionListWidget::onNewSession);
    serialSessionEditor.exec();
}

void SessionListWidget::onNewSSHSessionAction() {
    qDebug() << "onNewSSHSessionAction";
    SSHSessionEditor sshSessionEditor(this, currentItem()->text(0));
    connect(&sshSessionEditor, &SSHSessionEditor::newSession, this, &SessionListWidget::onNewSession);
    sshSessionEditor.exec();
}

void SessionListWidget::onEditSessionActioin() {
    qDebug() << "onEditSessionActioin";
    auto item = currentItem();
    QVariant var = item->data(0, Qt::UserRole);
    if (!var.canConvert<Session>()) {
        qDebug() << "current item is not session item";
        return;
    }
    Session session = var.value<Session>();
    auto parent = item->parent();
    if (session.sessionType == SESSION_TYPE::LOCAL_SHELL) {
        qDebug() << "edit local session";
        LocalSessionEditor localSessionEditor(this, parent->text(0), session);
        connect(&localSessionEditor, &LocalSessionEditor::updateSession, this, &SessionListWidget::onUpdateSession);
        localSessionEditor.exec();
    } else if (session.sessionType == SESSION_TYPE::SSH2) {
        qDebug() << "edit ssh session";
        SSHSessionEditor sshSessionEditor(this, parent->text(0), session);
        connect(&sshSessionEditor, &SSHSessionEditor::updateSession, this, &SessionListWidget::onUpdateSession);
        sshSessionEditor.exec();
    } else if (session.sessionType == SESSION_TYPE::SERIAL) {
        qDebug() << "edit serial session";
        SerialSessionEditor serialSessionEditor(this, parent->text(0), session);
        connect(&serialSessionEditor, &SerialSessionEditor::updateSession, this, &SessionListWidget::onUpdateSession);
        serialSessionEditor.exec();
    }
}

void SessionListWidget::onNewSession(const QString &categoryName, const Session &session) {
    auto *categoryItem = currentItem();
    auto *sessionItem = new QTreeWidgetItem(categoryItem, QStringList(session.sessionName.c_str()));
    QVariant sessionVar = QVariant::fromValue(session);
    sessionItem->setData(0, Qt::UserRole, sessionVar);
    sessionItem->setIcon(0, *_sessionIcon);
    categoryItem->addChild(sessionItem);
}

void
SessionListWidget::onUpdateSession(const QString &categoryName, const Session &oldSession, const Session &newSession) {
    auto *sessionItem = currentItem();
    QVariant sessionVar = QVariant::fromValue(newSession);
    sessionItem->setText(0, newSession.sessionName.c_str());
    sessionItem->setData(0, Qt::UserRole, sessionVar);
}

void SessionListWidget::onRemoveSessionAction() {
    qDebug() << "onRemoveSessionAction";
    auto item = currentItem();
    auto parent = item->parent();
    SettingManager::getInstance()->deleteSession(parent->text(0).toStdString(), item->text(0).toStdString());
    if (item->parent() == nullptr) {
        delete takeTopLevelItem(currentIndex().row());
    } else {
        delete item->parent()->takeChild(currentIndex().row());
    }
}

void SessionListWidget::onMoveUpSessionAction() {
    qDebug() << "onMoveUpSessionAction";
    QTreeWidgetItem *item = this->currentItem();
    QTreeWidgetItem *parent = item->parent();
    int index = parent->indexOfChild(item);
    if (index == 0) {
        return;
    }
    SettingManager::getInstance()->moveUpSession(parent->text(0).toStdString(), item->text(0).toStdString());
    QTreeWidgetItem *child = parent->takeChild(index);
    parent->insertChild(index - 1, child);
}

void SessionListWidget::onMoveDownSessionAction() {
    qDebug() << "onMoveDownSessionAction";
    QTreeWidgetItem *item = this->currentItem();
    QTreeWidgetItem *parent = item->parent();
    int index = parent->indexOfChild(item);
    if (index == parent->childCount() - 1) {
        return;
    }
    SettingManager::getInstance()->moveDownSession(parent->text(0).toStdString(), item->text(0).toStdString());
    QTreeWidgetItem *child = parent->takeChild(index);
    parent->insertChild(index+1, child);
}




