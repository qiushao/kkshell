//
// Created by min on 2020/8/7.
//

#include "base_terminal.h"

#include <QDebug>
#include "common/config/config_manager.h"

BaseTerminal::BaseTerminal(QWidget *parent) : QTermWidget(0, parent) {
    connect_ = false;

    ConfigManager *conf = ConfigManager::getInstance();

    //font
    QString fontFamily = conf->getCString("app", "fontFamily", "Monospace");
    int fontSize = conf->getInt("app", "fontSize", 18);
    font_ = new QFont();
    font_->setFamily(fontFamily);
    font_->setPointSize(fontSize);
    setTerminalFont(*font_);
    setHistorySize(128000);
    setTerminalSizeHint(false);
    QStringList env;
    env.append("TERM=xterm-256color");
    setEnvironment(env);

    //set color scheme
    QString scheme = conf->getCString("app", "colorScheme", "Tango");
    setColorScheme(scheme);

    //set scroll bar
    setScrollBarPosition(ScrollBarRight);

    QObject::connect(this, &QTermWidget::receivedData, this, &BaseTerminal::writeLog);
}

BaseTerminal::~BaseTerminal() {
    disableLogSession();
}

bool BaseTerminal::isConnect() {
    return connect_;
}

void BaseTerminal::logSession(const std::string &logPath) {
    logPath_ = logPath;
    logFp_ = fopen(logPath.c_str(), "wb+");
    logging_ = true;
}

void BaseTerminal::disableLogSession() {
    logging_ = false;
    fflush(logFp_);
    fclose(logFp_);
}

void BaseTerminal::writeLog(const char *buf, int len) {
    if (logging_) {
        fwrite(buf, len, 1, logFp_);
    }
}
