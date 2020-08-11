//
// Created by min on 2020/8/7.
//

#include "base_terminal.h"

#include <QDebug>
#include "common/config/config_manager.h"

BaseTerminal::BaseTerminal(QWidget *parent) : QTermWidget(0, parent) {
    connect_ = false;

    QFont font;
    ConfigManager *conf = ConfigManager::getInstance();

    //font
    QString fontFamily = conf->getCString("app", "fontFamily", "Monospace");
    int fontSize = conf->getInt("app", "fontSize", 18);
    font.setFamily(fontFamily);
    font.setPointSize(fontSize);
    setTerminalFont(font);

    //set color scheme
    QString scheme = conf->getCString("app", "colorScheme", "Tango");
    setColorScheme(scheme);

    //set scroll bar
    setScrollBarPosition(ScrollBarRight);
}

BaseTerminal::~BaseTerminal() {

}

void BaseTerminal::clear() {
    QTermWidget::clear();
}

bool BaseTerminal::isConnect() {
    return connect_;
}
