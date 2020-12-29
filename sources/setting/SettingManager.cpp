//
// Created by qiushao on 2020/10/27.
//

#include "SettingManager.h"

#include <QFile>
#include <QDebug>
#include <QtWidgets/QMessageBox>

#include "rapidjson/prettywriter.h"
#include "PlatformAdapter.h"
#include "FileUtils.h"

SettingManager *SettingManager::_instance = nullptr;

void SettingManager::copyDefaultSettings() {
    qDebug() << "copyDefaultSettings";
    QFile defaultSettings(":/settings.json");
    if (!defaultSettings.open(QIODevice::ReadOnly)) {
        qDebug() << "open :/settings.json error";
        return;
    }
    QByteArray buffer = defaultSettings.readAll();
    FileUtils::writeToFile(_settingPath, buffer.data(), buffer.length());
}

SettingManager::SettingManager() {
    _settingPath = PlatformAdapter::getSettingPath();
    _settingDir = FileUtils::getDirName(_settingPath);

    if (!FileUtils::isFileExist(_settingPath)) {
        FileUtils::createDirs(_settingDir);
        copyDefaultSettings();
    }
    _settingDoc.Parse(FileUtils::readFileContent(_settingPath).c_str());
    deserialize();
}

std::string SettingManager::getAppVersion() {
    return _settings.appVersion;
}

std::string SettingManager::getFontFamily() {
    return _settings.fontFamily;
}

int SettingManager::getFontSize() {
    return _settings.fontSize;
}

std::string SettingManager::getColorScheme() {
    return _settings.colorScheme;
}


void SettingManager::setFontFamily(const std::string &fontFamily) {
    _settings.fontFamily = fontFamily;
    serialize();
}

void SettingManager::setFontSize(int fontSize) {
    _settings.fontSize = fontSize;
    serialize();
}

void SettingManager::setColorScheme(const std::string &colorScheme) {
    _settings.colorScheme = colorScheme;
    serialize();
}


// session manage
const std::vector<SessionCategory>& SettingManager::getSessionCategoryList() {
    return _settings.sessionCategoryList;
}

void SettingManager::addSessionCategory(const std::string &categoryName) {
    SessionCategory* pCategory = findSessionCategory(categoryName);
    if (nullptr != pCategory) {
        qDebug() << "category " << categoryName.c_str() << " is already exist!";
        return;
    }
    SessionCategory category;
    category.categoryName = categoryName;
    _settings.sessionCategoryList.push_back(category);
    serialize();
}

void SettingManager::renameSessionCategory(const std::string &oldName, const std::string &newName) {
    SessionCategory* pCategory = findSessionCategory(oldName);
    if (nullptr == pCategory) {
        qDebug() << "category " << oldName.c_str() << " is not exist!";
        return;
    }
    pCategory->categoryName = newName;
    serialize();
}

bool SettingManager::deleteSessionCategory(const std::string &categoryName) {
    for (auto it = _settings.sessionCategoryList.begin(); it != _settings.sessionCategoryList.end(); ++it) {
        if (categoryName == it->categoryName) {
            if (it->sessionList.empty()) {
                _settings.sessionCategoryList.erase(it);
                serialize();
            } else {
                QMessageBox::critical(nullptr, "Error", "Category not empty!");
                qDebug() << "category " << categoryName.c_str() << " is not empty!";
                return false;
            }
            break;
        }
    }
    return true;
}

void SettingManager::moveUpSessionCategory(const std::string &categoryName) {
    for (auto it = _settings.sessionCategoryList.begin(); it != _settings.sessionCategoryList.end(); ++it) {
        if (categoryName == it->categoryName) {
            if ((it) == _settings.sessionCategoryList.begin()) {
                qDebug() << "category " << categoryName.c_str() << " is already top";
                return;
            }
            SessionCategory tmp = *(it - 1);
            *(it-1) = *it;
            *it = tmp;
            serialize();
            break;
        }
    }
}

void SettingManager::moveDownSessionCategory(const std::string &categoryName) {
    for (auto it = _settings.sessionCategoryList.begin(); it != _settings.sessionCategoryList.end(); ++it) {
        if (categoryName == it->categoryName) {
            if ((it) == (_settings.sessionCategoryList.end() - 1)) {
                qDebug() << "category " << categoryName.c_str() << " is already bottom";
                return;
            }
            SessionCategory tmp = *(it + 1);
            *(it+1) = *it;
            *it = tmp;
            serialize();
            break;
        }
    }
}

void SettingManager::addSession(const std::string &categoryName, const Session &session) {
    SessionCategory *pCategory = findSessionCategory(categoryName);
    if (nullptr == pCategory) {
        qDebug() << "category " << categoryName.c_str() << " is not exist!";
        return;
    }
    pCategory->sessionList.push_back(session);
    serialize();
}

void SettingManager::updateSession(const std::string &categoryName, const std::string &oldSessionName, const Session &session) {
    SessionCategory *pCategory = findSessionCategory(categoryName);
    if (nullptr == pCategory) {
        qDebug() << "category " << categoryName.c_str() << " is not exist!";
        return;
    }

    for(Session &tmpSession : pCategory->sessionList) {
        if (tmpSession.sessionName == oldSessionName) {
            qDebug() << "SettingManager updateSession ...";
            tmpSession = session;
            serialize();
            break;
        }
    }
}

void SettingManager::deleteSession(const std::string &categoryName, const std::string &sessionName) {
    SessionCategory *pCategory = findSessionCategory(categoryName);
    if (nullptr == pCategory) {
        qDebug() << "category " << categoryName.c_str() << " is not exist!";
        return;
    }

    for (auto it = pCategory->sessionList.begin(); it != pCategory->sessionList.end(); ++it) {
        if (it->sessionName == sessionName) {
            pCategory->sessionList.erase(it);
            serialize();
            break;
        }
    }
}

void SettingManager::moveUpSession(const std::string &categoryName, const std::string &sessionName) {
    SessionCategory *pCategory = findSessionCategory(categoryName);
    if (nullptr == pCategory) {
        qDebug() << "category " << categoryName.c_str() << " is not exist!";
        return;
    }

    for (auto it = pCategory->sessionList.begin(); it != pCategory->sessionList.end(); ++it) {
        if (sessionName == it->sessionName) {
            if ((it) == pCategory->sessionList.begin()) {
                qDebug() << "session " << sessionName.c_str() << " is already top";
                return;
            }
            Session tmp = *(it-1);
            *(it-1) = *it;
            *it = tmp;
            serialize();
            break;
        }
    }
}

void SettingManager::moveDownSession(const std::string &categoryName, const std::string &sessionName) {
    SessionCategory *pCategory = findSessionCategory(categoryName);
    if (nullptr == pCategory) {
        qDebug() << "category " << categoryName.c_str() << " is not exist!";
        return;
    }

    for (auto it = pCategory->sessionList.begin(); it != pCategory->sessionList.end(); ++it) {
        if (sessionName == it->sessionName) {
            if ((it) == (pCategory->sessionList.end() - 1)) {
                qDebug() << "session " << sessionName.c_str() << " is already bottom";
                return;
            }
            Session tmp = *(it+1);
            *(it+1) = *it;
            *it = tmp;
            serialize();
            break;
        }
    }
}

SessionCategory* SettingManager::findSessionCategory(const std::string &categoryName) {
    for (SessionCategory &category : _settings.sessionCategoryList) {
        if (category.categoryName == categoryName) {
            return &category;
        }
    }
    return nullptr;
}


// command button manage
const std::vector<CommandCategory> &SettingManager::getCommandCategoryList() {
    return _settings.commandCategoryList;
}

const std::vector<Command> &SettingManager::getCommandList(const std::string &categoryName) {
    for (const CommandCategory &category : _settings.commandCategoryList) {
        if (category.categoryName == categoryName) {
            return category.commandList;
        }
    }
}

void SettingManager::addCommandCategory(const std::string &categoryName) {
    CommandCategory* pCategory = findCommandCategory(categoryName);
    if (nullptr != pCategory) {
        qDebug() << "category " << categoryName.c_str() << " is already exist!";
        return;
    }
    CommandCategory category;
    category.categoryName = categoryName;
    _settings.commandCategoryList.push_back(category);
    serialize();
}

void SettingManager::renameCommandCategory(const std::string &oldName, const std::string &newName) {
    CommandCategory* pCategory = findCommandCategory(oldName);
    if (nullptr == pCategory) {
        qDebug() << "category " << oldName.c_str() << " is not exist!";
        return;
    }
    pCategory->categoryName = newName;
    serialize();
}

bool SettingManager::deleteCommandCategory(const std::string &categoryName) {
    for (auto it = _settings.commandCategoryList.begin(); it != _settings.commandCategoryList.end(); ++it) {
        if (categoryName == it->categoryName) {
            if (it->commandList.empty()) {
                _settings.commandCategoryList.erase(it);
                serialize();
                return true;
            } else {
                QMessageBox::critical(nullptr, "Error", "Category not empty!");
                qDebug() << "category " << categoryName.c_str() << " is not empty!";
            }
            break;
        }
    }
    return false;
}

void SettingManager::moveUpCommandCategory(const std::string &categoryName) {
    for (auto it = _settings.commandCategoryList.begin(); it != _settings.commandCategoryList.end(); ++it) {
        if (categoryName == it->categoryName) {
            if ((it) == _settings.commandCategoryList.begin()) {
                qDebug() << "category " << categoryName.c_str() << " is already top";
                return;
            }
            CommandCategory tmp = *(it - 1);
            *(it-1) = *it;
            *it = tmp;
            serialize();
            break;
        }
    }
}

void SettingManager::moveDownCommandCategory(const std::string &categoryName) {
    for (auto it = _settings.commandCategoryList.begin(); it != _settings.commandCategoryList.end(); ++it) {
        if (categoryName == it->categoryName) {
            if ((it) == (_settings.commandCategoryList.end() - 1)) {
                qDebug() << "category " << categoryName.c_str() << " is already bottom";
                return;
            }
            CommandCategory tmp = *(it + 1);
            *(it+1) = *it;
            *it = tmp;
            serialize();
            break;
        }
    }
}

void SettingManager::addCommand(const std::string &categoryName, const Command &command) {
    CommandCategory *pCategory = findCommandCategory(categoryName);
    if (nullptr == pCategory) {
        qDebug() << "category " << categoryName.c_str() << " is not exist!";
        return;
    }
    pCategory->commandList.push_back(command);
    serialize();
}

void SettingManager::updateCommand(const std::string &categoryName, const std::string &commandName, const Command &command) {
    CommandCategory *pCategory = findCommandCategory(categoryName);
    if (nullptr == pCategory) {
        qDebug() << "category " << categoryName.c_str() << " is not exist!";
        return;
    }

    for(Command &tmpCommand : pCategory->commandList) {
        if (tmpCommand.commandName == commandName) {
            tmpCommand = command;
            serialize();
            break;
        }
    }
}

void SettingManager::deleteCommand(const std::string &categoryName, const std::string &commandName) {
    CommandCategory *pCategory = findCommandCategory(categoryName);
    if (nullptr == pCategory) {
        qDebug() << "category " << categoryName.c_str() << " is not exist!";
        return;
    }

    for (auto it = pCategory->commandList.begin(); it != pCategory->commandList.end(); ++it) {
        if (it->commandName == commandName) {
            pCategory->commandList.erase(it);
            serialize();
            break;
        }
    }
}

void SettingManager::moveUpCommand(const std::string &categoryName, const std::string &commandName) {
    CommandCategory *pCategory = findCommandCategory(categoryName);
    if (nullptr == pCategory) {
        qDebug() << "category " << categoryName.c_str() << " is not exist!";
        return;
    }

    for (auto it = pCategory->commandList.begin(); it != pCategory->commandList.end(); ++it) {
        if (commandName == it->commandName) {
            if ((it) == pCategory->commandList.begin()) {
                qDebug() << "session " << commandName.c_str() << " is already top";
                return;
            }
            Command tmp = *(it-1);
            *(it-1) = *it;
            *it = tmp;
            serialize();
            break;
        }
    }
}

void SettingManager::moveDownCommand(const std::string &categoryName, const std::string &commandName) {
    CommandCategory *pCategory = findCommandCategory(categoryName);
    if (nullptr == pCategory) {
        qDebug() << "category " << categoryName.c_str() << " is not exist!";
        return;
    }

    for (auto it = pCategory->commandList.begin(); it != pCategory->commandList.end(); ++it) {
        if (commandName == it->commandName) {
            if ((it) == (pCategory->commandList.end() - 1)) {
                qDebug() << "session " << commandName.c_str() << " is already bottom";
                return;
            }
            Command tmp = *(it+1);
            *(it+1) = *it;
            *it = tmp;
            serialize();
            break;
        }
    }
}

CommandCategory* SettingManager::findCommandCategory(const std::string &categoryName) {
    for (CommandCategory &category : _settings.commandCategoryList) {
        if (category.categoryName == categoryName) {
            return &category;
        }
    }
    return nullptr;
}


// serialize and deserialize
void SettingManager::deserialize() {
    _settings.appVersion = _settingDoc["appVersion"].GetString();
    _settings.fontFamily = _settingDoc["fontFamily"].GetString();
    _settings.fontSize = _settingDoc["fontSize"].GetInt();
    _settings.colorScheme = _settingDoc["colorScheme"].GetString();

    rapidjson::Value& sessionCategoriesDom = _settingDoc["sessionCategories"];
    int categoryCount = sessionCategoriesDom.Size();
    for (int i = 0; i < categoryCount; ++i) {
        rapidjson::Value& categoryDom = sessionCategoriesDom[i];
        SessionCategory category;
        category.categoryName = categoryDom["categoryName"].GetString();
        rapidjson::Value& sessionsDom = categoryDom["sessions"];

        int sessionCount = sessionsDom.Size();
        for (int j = 0; j < sessionCount; ++j) {
            rapidjson::Value& sessionDom = sessionsDom[j];
            Session session;
            session.sessionName = sessionDom["sessionName"].GetString();
            session.sessionType = static_cast<SESSION_TYPE>(sessionDom["sessionType"].GetInt());

            if (SESSION_TYPE::SSH2 == session.sessionType) {
                session.host = sessionDom["host"].GetString();
                session.port = sessionDom["port"].GetInt();
                session.userName = sessionDom["userName"].GetString();
                session.authType = static_cast<SSH_AUTH_TYPE>(sessionDom["authType"].GetInt());
                session.passwd = sessionDom["passwd"].GetString();
                session.keyFile = sessionDom["keyFile"].GetString();

            } else if (SESSION_TYPE::SERIAL == session.sessionType) {
                session.dev = sessionDom["dev"].GetString();
                session.baudRate = sessionDom["baudRate"].GetInt();
                session.dataBits = sessionDom["dataBits"].GetInt();
                session.stopBits = sessionDom["stopBits"].GetInt();
                session.parity = sessionDom["parity"].GetInt();
                session.flowControl = sessionDom["flowControl"].GetInt();
            }

            category.sessionList.push_back(session);
        }

        _settings.sessionCategoryList.push_back(category);
    }


    rapidjson::Value& commandCategoriesDom = _settingDoc["commandCategories"];
    int commandCategoryCount = commandCategoriesDom.Size();
    for (int i = 0; i < commandCategoryCount; ++i) {
        rapidjson::Value& categoryDom = commandCategoriesDom[i];
        CommandCategory category;
        category.categoryName = categoryDom["categoryName"].GetString();
        rapidjson::Value& commandsDom = categoryDom["commands"];

        int commandCount = commandsDom.Size();
        for (int j = 0; j < commandCount; ++j) {
            rapidjson::Value& commandDom = commandsDom[j];
            Command command;
            command.commandName = commandDom["commandName"].GetString();
            command.commandString = commandDom["commandString"].GetString();

            category.commandList.push_back(command);
        }

        _settings.commandCategoryList.push_back(category);
    }
}

void SettingManager::serialize() {
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

    writer.StartObject();

    writer.Key("appVersion");
    writer.String(_settings.appVersion.c_str());

    writer.Key("fontFamily");
    writer.String(_settings.fontFamily.c_str());

    writer.Key("fontSize");
    writer.Int(_settings.fontSize);

    writer.Key("colorScheme");
    writer.String(_settings.colorScheme.c_str());

    writer.Key("sessionCategories");
    writer.StartArray();

    for (const SessionCategory& category : _settings.sessionCategoryList) {
        writer.StartObject();

        writer.Key("categoryName");
        writer.String(category.categoryName.c_str());

        writer.Key("sessions");
        writer.StartArray();
        for (const Session& session : category.sessionList) {
            writer.StartObject();

            writer.Key("sessionName");
            writer.String(session.sessionName.c_str());

            writer.Key("sessionType");
            writer.Int(session.sessionType);

            if (SESSION_TYPE::SSH2 == session.sessionType) {
                writer.Key("host");
                writer.String(session.host.c_str());

                writer.Key("port");
                writer.Int(session.port);

                writer.Key("authType");
                writer.Int(session.authType);

                writer.Key("userName");
                writer.String(session.userName.c_str());

                writer.Key("passwd");
                writer.String(session.passwd.c_str());

                writer.Key("keyFile");
                writer.String(session.keyFile.c_str());

            } else if (SESSION_TYPE::SERIAL == session.sessionType) {
                writer.Key("dev");
                writer.String(session.dev.c_str());

                writer.Key("baudRate");
                writer.Int(session.baudRate);

                writer.Key("dataBits");
                writer.Int(session.dataBits);

                writer.Key("stopBits");
                writer.Int(session.stopBits);

                writer.Key("parity");
                writer.Int(session.parity);

                writer.Key("flowControl");
                writer.Int(session.flowControl);
            }

            writer.EndObject(); // session end
        }
        writer.EndArray(); // sessions end

        writer.EndObject(); // session category end
    }

    writer.EndArray(); // session categories end


    writer.Key("commandCategories");
    writer.StartArray();
    for (const CommandCategory& category : _settings.commandCategoryList) {
        writer.StartObject();

        writer.Key("categoryName");
        writer.String(category.categoryName.c_str());

        writer.Key("commands");
        writer.StartArray();
        for (const Command& command : category.commandList) {
            writer.StartObject();

            writer.Key("commandName");
            writer.String(command.commandName.c_str());

            writer.Key("commandString");
            writer.String(command.commandString.c_str());

            writer.EndObject(); // command end
        }
        writer.EndArray(); // commands end

        writer.EndObject(); // command category end
    }

    writer.EndArray(); // command categories end



    writer.EndObject();

    FileUtils::writeToFile(_settingPath, buffer.GetString(), buffer.GetLength());
}





