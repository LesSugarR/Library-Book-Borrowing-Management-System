// Console.h
#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>
#include <fstream>
#include <filesystem>

class Console {
private:
    std::string dataDir;  // 存储data目录的路径

public:
    // 构造函数，初始化data目录
    Console() {
        // 默认将data目录放在当前目录下
        dataDir = "data";

        // 或者可以设置为构建目录下的data
        // dataDir = "cmake-build-debug-mingw/data";
    }

    // 设置data目录路径
    void setDataDir(const std::string& dir) {
        dataDir = dir;
        // 确保目录存在
        mkdir(dataDir);
    }

    // 获取data目录路径
    std::string getDataDir() const {
        return dataDir;
    }

    // 目录生成函数
    std::string rdir(const std::string& rid) const;
    std::string bdir(const std::string& bid) const;
    std::string hdir(const std::string& hid) const;
    void mkdir(const std::string& path) const;

    // 文件操作模板函数
    template <class T>
    void outf(const std::string& file, const T& t) {
        std::ofstream fout(file);
        if (!fout) throw std::runtime_error("无法打开文件: " + file);
        fout << t;
        fout.close();
    }

    template <class T>
    void inf(const std::string& file, T& t) {
        std::ifstream fin(file);
        if (!fin) throw std::runtime_error("无法打开文件: " + file);
        fin >> t;
        fin.close();
    }
};

extern Console con;

#endif // CONSOLE_H