//
// Created by qiushao on 2020/10/27.
//

#ifndef KKSHELL_SETTINGMANAGER_H
#define KKSHELL_SETTINGMANAGER_H

#include <string>
#include "rapidjson/document.h"
#include "Settings.h"

class SettingManager {
public:
    static inline SettingManager *getInstance() {
        if (_instance == nullptr) {
            _instance = new SettingManager();
        }
        return _instance;
    }

private:
    static SettingManager *_instance;
    SettingManager();
    SettingManager(const SettingManager &) {};
    SettingManager &operator=(const SettingManager &) { return *this; }

public:
    std::string getAppVersion();
    std::string getFontFamily();
    void setFontFamily(const std::string &fontFamily);
    int getFontSize();
    void setFontSize(int fontSize);
    std::string getColorScheme();
    void setColorScheme(const std::string &colorScheme);

    // session list
    void addSessionCategory(const std::string &categoryName);
    void renameSessionCategory(const std::string &oldName, const std::string &newName);
    bool deleteSessionCategory(const std::string &categoryName);
    void moveUpSessionCategory(const std::string &categoryName);
    void moveDownSessionCategory(const std::string &categoryName);

    void addSession(const std::string &categoryName, const Session &session);
    void updateSession(const std::string &categoryName, const std::string &oldSessionName, const Session &session);
    void deleteSession(const std::string &categoryName, const std::string &sessionName);
    void moveUpSession(const std::string &categoryName, const std::string &sessionName);
    void moveDownSession(const std::string &categoryName, const std::string &sessionName);

    const std::vector<SessionCategory>& getSessionCategoryList();

    // command button list
    void addCommandCategory(const std::string &categoryName);
    void renameCommandCategory(const std::string &oldName, const std::string &newName);
    bool deleteCommandCategory(const std::string &categoryName);
    void moveUpCommandCategory(const std::string &categoryName);
    void moveDownCommandCategory(const std::string &categoryName);

    void addCommand(const std::string &categoryName, const Command &command);
    void updateCommand(const std::string &categoryName, const std::string &commandName, const Command &commnad);
    void deleteCommand(const std::string &categoryName, const std::string &commandName);
    void moveUpCommand(const std::string &categoryName, const std::string &commandName);
    void moveDownCommand(const std::string &categoryName, const std::string &commandName);

    const std::vector<CommandCategory>& getCommandCategoryList();
    const std::vector<Command>& getCommandList(const std::string &categoryName);

private:
    SessionCategory* findSessionCategory(const std::string &categoryName);
    CommandCategory* findCommandCategory(const std::string &categoryName);
    void copyDefaultSettings();
    void deserialize();
    void serialize();

private:
    std::string _settingPath;
    std::string _settingDir;
    rapidjson::Document _settingDoc;
    Settings _settings;
};


#endif //KKSHELL_SETTINGMANAGER_H
