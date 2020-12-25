//
// Created by qiushao on 2020/10/28.
//

#ifndef KKSHELL_FILEUTILS_H
#define KKSHELL_FILEUTILS_H


#include <string>

class FileUtils {
public:
    static bool isFileExist(const std::string &path);
    static size_t getFileSize(const std::string &path);
    static std::string getDirName(const std::string &path);
    static std::string getBaseName(const std::string &path);
    static std::string readFileContent(const std::string &path);
    static bool createDirs(const std::string &path);
    static bool writeToFile(const std::string &path, const char *buffer, const size_t len);
};


#endif //KKSHELL_FILEUTILS_H
