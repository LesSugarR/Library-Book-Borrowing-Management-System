<div align="center">

# 🏛️ Library Book Borrowing Management System
### 图书借阅管理系统

<!-- 下面是 ASCII 艺术字 -->
  _      ____  ____  __  __  _____ 
 | |    |  _ \|  _ \|  \/  |/ ____|
 | |    | |_) | |_) | \  / | (___  
 | |    |  _ <|  _ <| |\/| |\___ \\
 | |____| |_) | |_) | |  | |____) |
 |______|____/|____/|_|  |_|_____/ 


<!-- 下面是徽章代码 -->
![C++](https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=c%2B%2B)![CMake](https://img.shields.io/badge/Build-CMake-064F8C?style=for-the-badge&logo=cmake)![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows)![IDE](https://img.shields.io/badge/IDE-CLion-black?style=for-the-badge&logo=clion)

<br/>

<p align="center">
  <b>🌟 一个基于 C++17 的现代化图书资源管理解决方案 (Windows 版)</b>
  <br />
  <br />
  <a href="#-系统演示">系统演示</a> •
  <a href="#-核心功能">核心功能</a> •
  <a href="#-如何运行">如何运行</a> •
  <a href="#-项目架构">项目架构</a>
</p>

</div>

---

## 📖 项目简介 (Introduction)

**Library-Book-Borrowing-Management-System** (LBBMS) 是一个运行在 Windows 平台下的高效控制台应用程序。它旨在解决小型图书馆或个人藏书管理中的痛点，通过精简的交互逻辑，实现图书的快速录入、精准查询以及借阅流程的数字化管理。

本项目采用 **C++ 17** 标准开发，利用 **CMake** 进行构建，完美适配 JetBrains CLion 环境。

---

## ⚡ 核心功能 (Features)

### 👤 用户管理模块
- [x] **身份鉴权**：支持管理员 (Admin) 与普通读者 (Reader) 双端登录。
- [x] **账户安全**：提供注册、密码修改及账户注销功能。

### 📚 图书资源中心
- **智能入库**：管理员可批量录入书籍（书名、作者、ISBN、分类）。
- **精准检索**：支持多维度搜索（按书名模糊匹配、按作者查找）。
- **库存监控**：实时显示书籍的在馆数量与借出状态。

### 🚀 借阅流转系统
- **一键借阅**：自动校验用户借阅权限与书籍库存。
- **归还处理**：简单的归还操作，系统自动更新库存状态。
- **流水记录**：(开发中) 完整的借阅历史日志查询。

---

## 🛠️ 技术栈 (Tech Stack)

| 核心技术 | 说明 |
| :--- | :--- |
| **C++ 17** | 核心编程语言，使用了 STL 容器、智能指针等特性 |
| **CMake** | 构建系统 (Version ≥ 3.20) |
| **Windows Console** | 针对 Windows 控制台优化的交互界面 |
| **File I/O** | 本地文件持久化存储 |

---

## 💻 如何运行 (How to Run)

### 环境要求
*   **操作系统**: Windows 10 / 11
*   **开发工具**: 推荐使用 JetBrains CLion (或 VS Code + MinGW)
*   **编译器**: MinGW (GCC) 或 MSVC

### 1. 克隆仓库
```bash
git clone https://github.com/YourName/Library-Book-Borrowing-Management-System.git
