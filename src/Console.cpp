// Console.cpp
#include "../include/Console.h"
#include <sys/stat.h>
#include <direct.h>
#include <filesystem>

std::string Console::rdir(const std::string& rid) const {
    // 构建完整路径：dataDir/reader/rid/
    std::string dir = dataDir + "/reader/" + rid + "/";
    mkdir(dir);
    return dir;
}

std::string Console::bdir(const std::string& bid) const {
    std::string dir = dataDir + "/book/" + bid + "/";
    mkdir(dir);
    return dir;
}

std::string Console::hdir(const std::string& hid) const {
    std::string dir = dataDir + "/history/" + hid + "/";
    mkdir(dir);
    return dir;
}

void Console::mkdir(const std::string& path) const {
    // 创建多级目录
    std::filesystem::create_directories(path);

    // 或者使用传统方法（兼容性更好）
    /*
    size_t pos = 0;
    do {
        pos = path.find_first_of("/\\", pos + 1);
        std::string subdir = path.substr(0, pos);

        #ifdef _WIN32
            ::_mkdir(subdir.c_str());
        #else
            ::mkdir(subdir.c_str(), 0755);
        #endif
    } while (pos != std::string::npos);
    */
}