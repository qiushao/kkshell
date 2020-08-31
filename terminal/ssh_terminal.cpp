//
// Created by min on 2020/8/7.
//

#include "ssh_terminal.h"
#include <unistd.h>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include <QtCore/QEventLoop>

SSHTerminal::SSHTerminal(const SSHSettings &settings, QWidget *parent) : BaseTerminal(parent) {
    startShellProgram();
    settings_ = settings;
}

SSHTerminal::~SSHTerminal() {
}

void SSHTerminal::connect() {
    QString shellFile = createShellFile();
    QString strTxt = "expect -f " + shellFile + "\n";
    sendText(strTxt);

    QTime dieTime = QTime::currentTime().addMSecs(200);
    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    if (isInRemoteServer()) {
        qDebug() << "connect server success";
        connect_ = true;
    } else {
        qDebug() << "connect server failed!!!";
        connect_ = false;
    }
}

void SSHTerminal::disconnect() {
    if (!connect_) {
        return;
    }
    connect_ = false;
}

/*******************************************************************************
 1. @函数:    isInRemoteServer
 2. @作者:    ut000610 daizhengwen
 3. @日期:    2020-08-11
 4. @说明:    是否在远程服务器中
*******************************************************************************/
bool SSHTerminal::isInRemoteServer()
{
    int pid = getForegroundPID();
    qDebug() << "getShellPID = " << pid << endl;
    if (pid <= 0) {
        return false;
    }
    QString pidFilepath = "/proc/" + QString::number(pid) + "/comm";
    QFile pidFile(pidFilepath);
    if (pidFile.exists()) {
        pidFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString commString(pidFile.readLine());
        pidFile.close();
        if ("expect" == commString.trimmed()) {
            return true;
        }
    }
    return false;
}

/*******************************************************************************
 1. @函数:    getRandString
 2. @作者:    ut000439 wangpeili
 3. @日期:    2020-08-11
 4. @说明:    获取随机字符串
*******************************************************************************/
static QString getRandString()
{
    int max = 6;  //字符串长度
    QString tmp = QString("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    QString str;
    QTime t;
    t = QTime::currentTime();
    qsrand(t.msec() + t.second() * 1000);
    for (int i = 0; i < max; i++) {
        int len = qrand() % tmp.length();
        str[i] = tmp.at(len);
    }
    return str;
}

/*******************************************************************************
 1. @函数:    createShellFile
 2. @作者:    ut000610 戴正文
 3. @日期:    2020-07-31
 4. @说明:    创建连接远程的的临时shell文件
*******************************************************************************/
QString SSHTerminal::createShellFile()
{
    // 首先读取通用模板
    QFile sourceFile(":/script/ssh_login.sh");
    QString fileString;
    // 打开文件
    if (sourceFile.open(QIODevice::ReadOnly)) {
        //读取文件
        fileString = sourceFile.readAll();
        // 关闭文件
        sourceFile.close();
    }

    // 用远程管理数据替换文件内的关键字
    fileString.replace("<<USER>>", settings_.user.c_str());
    fileString.replace("<<SERVER>>", settings_.host.c_str());
    fileString.replace("<<PORT>>", QString::number(settings_.port));
    // 根据是否有证书，替换关键字
    if (settings_.authType == "passwd") {
        fileString.replace("<<PRIVATE_KEY>>", "");
        QRegExp rx("([\"$\\\\])");
        QString password = settings_.passwd.c_str();
        password.replace(rx, "\\\\\\1");
        fileString.replace("<<PASSWORD>>", password);
        fileString.replace("<<AUTHENTICATION>>", "no");
    } else {
        fileString.replace("<<PRIVATE_KEY>>", QString("-i " + QString(settings_.keyFile.c_str())));
        fileString.replace("<<PASSWORD>>", "");
        fileString.replace("<<AUTHENTICATION>>", "yes");
    }
    // 添加远程提示
    QString remote_command = "echo " + tr("Make sure that rz and sz commands have been installed in the server before right clicking to upload and download files.") + " && ";
    fileString.replace("<<REMOTE_COMMAND>>", remote_command);

    // 创建临时文件
    QString toFileStr = "/tmp/kkshell-" + getRandString();
    QFile toFile(toFileStr);
    toFile.open(QIODevice::WriteOnly | QIODevice::Text);
    // 写文件
    toFile.write(fileString.toUtf8());
    // 退出文件
    toFile.close();
    return toFileStr;
}