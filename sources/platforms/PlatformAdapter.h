//
// Created by qiushao on 2020/10/27.
//

#ifndef KKSHELL_PLATFORMADAPTER_H
#define KKSHELL_PLATFORMADAPTER_H


#include <string>

namespace PlatformAdapter {
    std::string expandUserHomePath(std::string path);
    std::string getSettingPath();
}


#endif //KKSHELL_PLATFORMADAPTER_H
