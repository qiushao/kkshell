//
// Created by min on 2020/8/6.
//

#include "config_manager.h"
#include <libgen.h>
#include <zconf.h>

ConfigManager *ConfigManager::instance = new ConfigManager();
static const char *configPath = "~/.config/kkshell/ini/settings.ini";

static bool isFleExist(const char *path) {
    return (access(path, 0) != -1);
}

static std::string expand_user(std::string path) {
    if (not path.empty() and path[0] == '~') {
        char const *home = getenv("HOME");
        if (home) {
            path.replace(0, 1, home);
        }
    }
    return path;
}

static std::string dirName(const char *path) {
    char buffer[PATH_MAX];
    strcpy(buffer, path);
    char *name = dirname(buffer);
    return std::string(name);
}


ConfigManager::ConfigManager() {
    absConfigPath_ = expand_user(configPath);
    absConfigDir_ = dirName(absConfigPath_.c_str());

    mIni.SetUnicode();
    mIni.SetMultiLine();

    if (!loadDataFromDB()) {
        loadDefaultConfig();
    }
}

bool ConfigManager::loadDataFromDB() {
    SI_Error ret = mIni.LoadFile(absConfigPath_.c_str());
    if (SI_OK != ret) {
        printf("loadDataFromDB error, reload default config\n");
        fflush(stdout);
        return false;
    }

    std::string version = getString("app", "version", "0");
    if ("0" == version) {
        printf("loadDataFromDB error, reload default config\n");
        fflush(stdout);
        return false;
    }

    return true;
}

void ConfigManager::loadDefaultConfig() {
    setCString("app", "version", "1.0.00");
    setCString("app", "fontFamily", "Monospace");
    setInt("app", "fontSize", 18);
    setCString("app", "colorScheme", "Tango");

    setCString("button_bar", "01-disable-log", "echo set log_level 6 > /proc/konka/LogImpl\\r");
    setCString("button_bar", "02-remount", "mount -o rw,remount /vendor\\r");
    if (!save()) {
        printf("save config error\n");
        fflush(stdout);
    }
}

bool ConfigManager::getBool(const char *section, const char *key, bool defaultValue) {
    return mIni.GetBoolValue(section, key, defaultValue);
}

static std::vector<bool> string2BooleanArray(const char *str) {
    std::vector<bool> array;
    char *p = const_cast<char *>(str);

    while (*str != '\0') {
        while (*str == ',' || isspace(static_cast<int>(*str))) str++;
        p = const_cast<char *>(str);
        while (*p != '\0' && *p != ',') p++;

        char tmp[32] = {0};

        if (p > str) {
            strncpy(tmp, str, p - str);

            if (!strcasecmp(tmp, "y")
                || !strcasecmp(tmp, "yes")
                || !strcasecmp(tmp, "true")
                || !strcasecmp(tmp, "1"))
                array.push_back(true);
            if (!strcasecmp(tmp, "n")
                || !strcasecmp(tmp, "no")
                || !strcasecmp(tmp, "false")
                || !strcasecmp(tmp, "0"))
                array.push_back(false);
        }

        if (*p == '\0') break;
        str = ++p;
    }

    return array;
}

std::vector<bool> ConfigManager::getBoolArray(const char *section, const char *key) {
    std::vector<bool> result;
    const char *str = mIni.GetValue(section, key, NULL);
    if (str != NULL) {
        result = string2BooleanArray(str);
    }
    return result;
}

int32_t ConfigManager::getInt(const char *section, const char *key, int32_t defaultValue) {
    return (int32_t) mIni.GetLongValue(section, key, defaultValue);
}

std::vector<int32_t> string2IntArray(const char *str) {
    std::vector<int32_t> array;
    char *p = const_cast<char *>(str);

    while (*str != '\0') {
        while (*str == ',' || isspace(static_cast<int>(*str))) str++;
        p = const_cast<char *>(str);
        while (*p != '\0' && *p != ',') p++;

        char tmp[32] = {0};

        if (p > str) {
            strncpy(tmp, str, p - str);
            int32_t value;
            if (!strncasecmp(tmp, "0x", 2)) {
                if (EOF == sscanf(tmp, "%x", &value)) {
                    printf("string2IntArray error: %s\n", str);
                    fflush(stdout);
                }
            } else {
                if (EOF == sscanf(tmp, "%d", &value)) {
                    printf("string2IntArray error: %s\n", str);
                }
            }
            array.push_back(value);
        }

        if (*p == '\0') break;
        str = ++p;
    }

    return array;
}

std::vector<int32_t> ConfigManager::getIntArray(const char *section, const char *key) {
    std::vector<int32_t> result;
    const char *str = mIni.GetValue(section, key, NULL);
    if (str != NULL) {
        result = string2IntArray(str);
    }
    return result;
}

double ConfigManager::getDouble(const char *section, const char *key, double defaultValue) {
    return mIni.GetDoubleValue(section, key, defaultValue);;
}

std::vector<double> string2DoubleArray(const char *str) {
    std::vector<double> array;
    char *p = const_cast<char *>(str);

    while (*str != '\0') {
        while (*str == ',' || isspace(static_cast<int>(*str))) str++;
        p = const_cast<char *>(str);
        while (*p != '\0' && *p != ',') p++;

        char tmp[32] = {0};

        if (p > str) {
            strncpy(tmp, str, p - str);
            double value;
            if (EOF == sscanf(tmp, "%lf", &value)) {
                printf("string2DoubleArray error: %s\n", str);
            }
            array.push_back(value);
        }

        if (*p == '\0') break;
        str = ++p;
    }

    return array;
}

std::vector<double> ConfigManager::getDoubleArray(const char *section, const char *key) {
    std::vector<double> result;
    const char *str = mIni.GetValue(section, key, NULL);
    if (str != NULL) {
        result = string2DoubleArray(str);
    }
    return result;
}

const char *ConfigManager::getCString(const char *section, const char *key, const char *defaultValue) {
    return mIni.GetValue(section, key, defaultValue);
}

std::string ConfigManager::getString(const char *section, const char *key, const char *defaultValue) {
    return mIni.GetValue(section, key, defaultValue);
}


void ConfigManager::setBool(const char *section, const char *key, bool value) {
    mIni.SetBoolValue(section, key, value);
    save();
}

void ConfigManager::setInt(const char *section, const char *key, int32_t value) {
    mIni.SetLongValue(section, key, value);
    save();
}

void ConfigManager::setDouble(const char *section, const char *key, double value) {
    mIni.SetDoubleValue(section, key, value);
    save();
}

void ConfigManager::setCString(const char *section, const char *key, const char *value) {
    mIni.SetValue(section, key, value);
    save();
}

bool ConfigManager::save() {
    if (!isFleExist(absConfigDir_.c_str())) {
        char cmd[256];
        memset(cmd, 0, sizeof(cmd));
        sprintf(cmd, "mkdir -p %s", absConfigDir_.c_str());
        system(cmd);
        system("sync");
    }
    SI_Error ret = mIni.SaveFile(absConfigPath_.c_str());
    return ret == SI_OK;
}

std::vector<std::string> ConfigManager::getAllSections() {
    CSimpleIniA::TNamesDepend sections;
    mIni.GetAllSections(sections);
    std::vector<std::string> sectionList;
    for(CSimpleIniA::Entry const &entry : sections) {
        sectionList.push_back(std::string(entry.pItem));
    }
    return sectionList;
}

std::vector<std::string> ConfigManager::getSectionKeys(const char *section) {
    CSimpleIniA::TNamesDepend keys;
    mIni.GetAllKeys(section, keys);
    std::vector<std::string> keysList;
    for(CSimpleIniA::Entry const &entry : keys) {
        keysList.push_back(std::string(entry.pItem));
    }
    return keysList;
}
