//
// Created by qiushao on 2020/8/7.
//

#include "BaseTerminal.h"

#include <QDebug>
#include "setting/SettingManager.h"

BaseTerminal::BaseTerminal(QWidget *parent) : QTermWidget(0, parent) {
    _connect = false;

    SettingManager *conf = SettingManager::getInstance();

    //font
    std::string fontFamily = conf->getFontFamily();
    int fontSize = conf->getFontSize();
    font_ = new QFont();
    font_->setFamily(QString::fromStdString(fontFamily));
    font_->setPointSize(fontSize);
    setTerminalFont(*font_);
    setHistorySize(128000);
    setTerminalSizeHint(false);
    QStringList env;
    env.append("TERM=xterm-256color");
    setEnvironment(env);

    //set color scheme
    std::string scheme = conf->getColorScheme();
    setColorScheme(QString::fromStdString(scheme));

    //set scroll bar
    setScrollBarPosition(ScrollBarRight);

    QObject::connect(this, &QTermWidget::onNewLine, this, &BaseTerminal::onNewLine);
    QObject::connect(this, &QTermWidget::receivedData, this, &BaseTerminal::onHexData);
}

BaseTerminal::~BaseTerminal() {
    disableLogSession();
}

bool BaseTerminal::isConnect() {
    return _connect;
}

void BaseTerminal::logSession(const std::string &logPath) {
    logPath_ = logPath;
    logFp_ = fopen(logPath.c_str(), "wb+");
    logging_ = true;
}

void BaseTerminal::disableLogSession() {
    logging_ = false;
    if (logFp_) {
        fflush(logFp_);
        fclose(logFp_);
        logFp_ = nullptr;
    }
}

void BaseTerminal::onNewLine(const QString &line) {
    if (logging_) {
        fwrite(line.toStdString().c_str(), line.toStdString().length(), 1, logFp_);
        fwrite("\n", 1, 1, logFp_);
    }
}

bool BaseTerminal::isLoggingSession() {
    return logging_;
}


void BaseTerminal::logHexSession(const std::string &logPath) {
    logHexPath_ = logPath;
    logHexFp_ = fopen(logPath.c_str(), "wb+");
    loggingHex_ = true;
}

void BaseTerminal::disableLogHexSession() {
    loggingHex_ = false;
    if (logHexFp_) {
        fflush(logHexFp_);
        fclose(logHexFp_);
        logHexFp_ = nullptr;
    }
}

void BaseTerminal::onHexData(const char *buf, int len) {
    if (loggingHex_) {
        fwrite(buf, len, 1, logHexFp_);
    }
}

bool BaseTerminal::isLoggingHexSession() {
    return loggingHex_;
}
