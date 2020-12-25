//
// Created by qiushao on 2020/10/27.
//

#include "PlatformAdapter.h"

namespace PlatformAdapter {
    std::string expandUserHomePath(std::string path) {
        if (not path.empty() and path[0] == '~') {
            char const *home = getenv("HOME");
            if (home) {
                path.replace(0, 1, home);
            }
        }
        return path;
    }

    std::string getSettingPath() {
        return expandUserHomePath("~/.config/kkshell/settings.json");
    }
}