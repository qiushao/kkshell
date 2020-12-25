//
// Created by min on 2020/10/28.
//

#include <fstream>
#include <sstream>
#include <cstring>
#include <climits>
#include <unistd.h>
#include <libgen.h>
#include "FileUtils.h"

bool FileUtils::isFileExist(const std::string &path) {
    //FIXED ME: not support windows
    return (access(path.c_str(), 0) != -1);
}

size_t FileUtils::getFileSize(const std::string &path) {
    std::ifstream fin(path);
    if (fin.is_open()) {
        fin.seekg(0, std::ios::end);
        size_t size = fin.tellg();
        fin.close();
        return size;
    }
    return 0;
}

std::string FileUtils::getDirName(const std::string &path) {
    //FIXED ME: not support windows
    char buffer[PATH_MAX];
    strcpy(buffer, path.c_str());
    char *name = dirname(buffer);
    return std::string(name);
}

std::string FileUtils::getBaseName(const std::string &path) {
    //FIXED ME: not support windows
    char buffer[PATH_MAX];
    strcpy(buffer, path.c_str());
    char *name = basename(buffer);
    return std::string(name);
}

std::string FileUtils::readFileContent(const std::string &path) {
    std::ifstream in(path);
    std::ostringstream tmp;
    tmp << in.rdbuf();
    return tmp.str();
}

bool FileUtils::createDirs(const std::string &path) {
    char cmd[256];
    memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, "mkdir -p %s", path.c_str());
    system(cmd);
    system("sync");
    return true;
}

bool FileUtils::writeToFile(const std::string &path, const char *buffer, const size_t len) {
    FILE *out = fopen(path.c_str(), "w");
    if (out == nullptr) {
        fprintf(stderr, "open %s error\n", path.c_str());
        return false;
    }
    fwrite(buffer, len, 1, out);
    fflush(out);
    fclose(out);
    return true;
}
