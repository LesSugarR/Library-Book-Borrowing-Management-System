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
// 添加 rmdir 函数的实现
void Console::rmdir(const std::string& path) const {
    namespace fs = std::filesystem;

    try {
        if (fs::exists(path)) {
            // 如果路径存在且是目录，则删除整个目录（包括子目录和文件）
            if (fs::is_directory(path)) {
                fs::remove_all(path);
            } else {
                // 如果是文件，直接删除
                fs::remove(path);
            }
        } else {
            // 路径不存在，可能是正常的（比如已经删除过了）
            // 可以选择什么都不做，或者记录日志
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("删除目录失败: " + path + " - " + e.what());
    }
}