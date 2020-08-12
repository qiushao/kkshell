//
// Created by min on 2020/8/6.
//

#ifndef KKSHELL_CONFIG_MANAGER_H
#define KKSHELL_CONFIG_MANAGER_H

#include <vector>
#include "thirdparty/simpleini/SimpleIni.h"

class ConfigManager {
public:
    static inline ConfigManager *getInstance() {
        return instance;
    }

private:
    static ConfigManager *instance;

    ConfigManager();

    ConfigManager(const ConfigManager &) {}

    ConfigManager &operator=(const ConfigManager &) { return *this; }

public:
    std::vector<std::string> getAllSections();
    std::vector<std::string> getSectionKeys(const char *section);
    bool getBool(const char *section, const char *key, bool defaultValue = false);
    void setBool(const char *section, const char *key, bool value);

    std::vector<bool> getBoolArray(const char *section, const char *key);

    int32_t getInt(const char *section, const char *key, int32_t defaultValue = 0);
    void setInt(const char *section, const char *key, int32_t value);

    std::vector<int32_t> getIntArray(const char *section, const char *key);

    double getDouble(const char *section, const char *key, double defaultValue = 0.0);
    void setDouble(const char *section, const char *key, double value);

    std::vector<double> getDoubleArray(const char *section, const char *key);

    const char *getCString(const char *section, const char *key, const char *defaultValue = "");
    void setCString(const char *section, const char *key, const char *value);

    std::string getString(const char *section, const char *key, const char *defaultValue = "");

    void deleteSection(const char *section);
    void deleteKey(const char *section, const char *key);

private:
    bool loadDataFromDB();
    void loadDefaultConfig();
    bool save();

private:
    CSimpleIniA mIni;
    std::string absConfigPath_;
    std::string absConfigDir_;
};


#endif //KKSHELL_CONFIG_MANAGER_H
