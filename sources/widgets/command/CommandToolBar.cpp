//
// Created by qiushao on 2020/12/12.
//

#include <QDebug>
#include "CommandToolBar.h"
#include "widgets/command/CommandButtonEditor.h"
#include "widgets/command/CommandCategoryEditor.h"
#include "setting/SettingManager.h"

CommandToolBar::CommandToolBar(QWidget *parent) : QToolBar(parent) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    initMenu();
    initWidget();
}

CommandToolBar::~CommandToolBar() {

}

void CommandToolBar::initMenu() {
    _commandMenu = new QMenu(this);
    _newCommandAction = new QAction(tr("New Command"), this);
    connect(_newCommandAction, &QAction::triggered, this, &CommandToolBar::onNewCommandAction);
    _commandMenu->addAction(_newCommandAction);

    _editCommandAction = new QAction(tr("Edit Command"), this);
    connect(_editCommandAction, &QAction::triggered, this, &CommandToolBar::onEditCommandAction);
    _commandMenu->addAction(_editCommandAction);

    _removeCommandAction = new QAction(tr("Remove Command"), this);
    connect(_removeCommandAction, &QAction::triggered, this, &CommandToolBar::onRemoveCommandAction);
    _commandMenu->addAction(_removeCommandAction);


    _commandToolBarMenu = new QMenu(this);
    _commandToolBarMenu->addAction(_newCommandAction);
    _commandToolBarMenu->addSeparator();

    _newCommandCategoryAction = new QAction(tr("New Command Category"), this);
    connect(_newCommandCategoryAction, &QAction::triggered, this, &CommandToolBar::onNewCommandCategoryAction);
    _commandToolBarMenu->addAction(_newCommandCategoryAction);

    _editCurrentCategoryAction = new QAction(tr("Edit Current Command Category"), this);
    connect(_editCurrentCategoryAction, &QAction::triggered, this, &CommandToolBar::onEditCurrentCategoryAction);
    _commandToolBarMenu->addAction(_editCurrentCategoryAction);

    _removeCurrentCategoryAction = new QAction(tr("Remove Current Command Category"), this);
    connect(_removeCurrentCategoryAction, &QAction::triggered, this, &CommandToolBar::onRemoveCurrentCategoryAction);
    _commandToolBarMenu->addAction(_removeCurrentCategoryAction);

    connect(this, &CommandToolBar::customContextMenuRequested, this, &CommandToolBar::onPopMenu);
}

void CommandToolBar::initWidget() {
    _commandCategorySelect = new QComboBox(this);
    addWidget(_commandCategorySelect);

    std::vector<CommandCategory> commandCategories = SettingManager::getInstance()->getCommandCategoryList();
    for (const CommandCategory &category : commandCategories) {
        _commandCategorySelect->addItem(category.categoryName.c_str());
    }

    selectCategory(commandCategories[0].categoryName.c_str());
    QObject::connect(_commandCategorySelect, &QComboBox::currentTextChanged, this, &CommandToolBar::selectCategory);
}

void CommandToolBar::selectCategory(const QString &categoryName) {
    _currentCategory = categoryName;
    auto actionList = actions();
    for (int i = 1; i < actionList.size(); ++i) {
        auto action = (CommandButton*)actionList.at(i);
        disconnect(action, &CommandButton::clicked, this, &CommandToolBar::onCommandButtonClicked);
        removeAction(action);
        action->setParent(nullptr);
        delete action;
    }

    auto commandList = SettingManager::getInstance()->getCommandList(categoryName.toStdString());
    for (const Command &command : commandList) {
        addSeparator();
        auto *action = new CommandButton(command, this);
        connect(action, &CommandButton::clicked, this, &CommandToolBar::onCommandButtonClicked);
        addAction(action);
    }
}

void CommandToolBar::onPopMenu(const QPoint &point) {
    auto action = actionAt(point);
    if (action != nullptr) {
        _selectedCommand = dynamic_cast<CommandButton *>(action);
        _commandMenu->exec(QCursor::pos());
    } else {
        _commandToolBarMenu->exec(QCursor::pos());
    }
}

void CommandToolBar::onNewCommandAction() {
    CommandButtonEditor editor(this, _currentCategory);
    connect(&editor, &CommandButtonEditor::reloadCommandCategory, this, &CommandToolBar::selectCategory);
    editor.exec();
}

void CommandToolBar::onEditCommandAction() {
    CommandButtonEditor editor(this, _currentCategory, _selectedCommand->command());
    connect(&editor, &CommandButtonEditor::reloadCommandCategory, this, &CommandToolBar::selectCategory);
    editor.exec();
}

void CommandToolBar::onRemoveCommandAction() {
    SettingManager::getInstance()->deleteCommand(_currentCategory.toStdString(), _selectedCommand->command().commandName);
    selectCategory(_currentCategory);
}

void CommandToolBar::onCommandButtonClicked(const Command &command) {
    emit sendCommand(command);
}

void CommandToolBar::onNewCommandCategoryAction() {
    CommandCategoryEditor editor(this);
    connect(&editor, &CommandCategoryEditor::newCommandCategory, this, &CommandToolBar::onNewCommandCategory);
    editor.exec();
}

void CommandToolBar::onEditCurrentCategoryAction() {
    CommandCategoryEditor editor(this, _currentCategory);
    connect(&editor, &CommandCategoryEditor::renameCommandCategory, this, &CommandToolBar::onUpdateCommandCategory);
    editor.exec();
}

void CommandToolBar::onRemoveCurrentCategoryAction() {
    auto ret = SettingManager::getInstance()->deleteCommandCategory(_currentCategory.toStdString());
    if (ret) {
        _commandCategorySelect->removeItem(_commandCategorySelect->currentIndex());
    }
}

void CommandToolBar::onUpdateCommandCategory(const QString &oldCategoryName, const QString &newCategoryName) {
    _commandCategorySelect->setItemText(_commandCategorySelect->currentIndex(), newCategoryName);
}

void CommandToolBar::onNewCommandCategory(const CommandCategory &category) {
    _commandCategorySelect->addItem(category.categoryName.c_str());
}

