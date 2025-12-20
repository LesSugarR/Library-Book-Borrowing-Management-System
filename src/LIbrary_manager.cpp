//
// Created by LesSugarR on 25-12-10.
//


// Library_manager.cpp
// 第一部分：图书管理功能（上架、下架、信息修改）
// 第二部分：借阅管理功能 

#include "../include/Library.h"
#include "../include/Book.h"
#include <iostream>
#include <string>
#include <limits>
#include <iomanip>

using namespace std;
using namespace LibraryBookSystem;

// 辅助函数 
// 清空输入缓冲区
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// 显示分隔线
void printSeparator() {
    cout << "══════════════════════════════════════════════════════════════════════" << endl;
}

// 显示标题
void printTitle(const string& title) {
    printSeparator();
    cout << "                       " << title << endl;
    printSeparator();
}

// 暂停并等待用户按键
void pressAnyKeyToContinue() {
    cout << "\n按任意键继续...";
    clearInputBuffer();
    cin.get();
}

// 获取用户输入的整数（有验证）
int getIntegerInput(const string& prompt, int min = INT_MIN, int max = INT_MAX) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value < min || value > max) {
            cout << "输入无效，请输入 " << min << " 到 " << max << " 之间的整数" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            cin.ignore(); // 清除换行符
            return value;
        }
    }
}

// 获取用户输入的字符串（非空）
string getStringInput(const string& prompt, bool allowEmpty = false) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        if (!allowEmpty && input.empty()) {
            cout << "输入不能为空，请重新输入" << endl;
        } else {
            return input;
        }
    }
}

// 获取用户确认
bool getConfirmation(const string& prompt) {
    cout << prompt << " (y/n): ";
    char choice;
    cin >> choice;
    cin.ignore(); // 清除换行符
    return (choice == 'y' || choice == 'Y');
}

//图书上架功能

// 显示上架菜单
void showAddBookMenu() {
    printTitle("图书上架");
    cout << "请选择上架方式：" << endl;
    cout << "1. 新增图书（全新录入）" << endl;
    cout << "2. 增加库存（已有图书）" << endl;
    cout << "0. ↩返回主菜单" << endl;
    cout << "══════════════════════════════════════════════════════════════════════" << endl;
}

// 新增图书（完整录入）
void addNewBook(Library& lib) {
    printTitle("新增图书（完整录入）");
    
    try {
        cout << "请填写图书信息：" << endl;
        cout << "（带 * 号为必填项）" << endl;
        
        // 获取图书ID
        string bid = getStringInput("* 图书ID: ");
        
        // 检查图书是否已存在
        if (lib.op.bidExist(bid)) {
            cout << "图书ID已存在！" << endl;
            cout << "  您是否想要：\n";
            cout << "  1. 增加该图书库存\n";
            cout << "  2. 重新输入其他ID\n";
            int choice = getIntegerInput("请选择: ", 1, 2);
            
            if (choice == 1) {
                int num = getIntegerInput("请输入要增加的数量: ", 1, 1000);
                lib.mop.addBook(bid, num);
                cout << "库存增加成功！" << endl;
            }
            return;
        }
        
        // 获取其他信息
        string bname = getStringInput("* 书名: ");
        string author = getStringInput("* 作者: ");
        string press = getStringInput("* 出版社: ");
        int totalNum = getIntegerInput("* 总数量: ", 1, 1000);
        
        // 获取借出数量（新书默认为0）
        int borrowNum = 0;
        cout << "已借出数量（新书默认为0）: ";
        string temp;
        getline(cin, temp);
        if (!temp.empty()) {
            try {
                borrowNum = stoi(temp);
                if (borrowNum < 0 || borrowNum > totalNum) {
                    cout << "⚠️ 借出数量无效，将设为0" << endl;
                    borrowNum = 0;
                }
            } catch (...) {
                cout << "输入无效，将设为0" << endl;
            }
        }
        
        // 入库时间
        string inTime = getStringInput("入库时间（留空自动生成）: ", true);
        
        // 显示确认信息
        cout << "\n════════════ 请确认图书信息 ════════════" << endl;
        cout << "图书ID: " << bid << endl;
        cout << "书名: " << bname << endl;
        cout << "作者: " << author << endl;
        cout << "出版社: " << press << endl;
        cout << "总数量: " << totalNum << endl;
        cout << "借出数量: " << borrowNum << endl;
        cout << "可借数量: " << (totalNum - borrowNum) << endl;
        if (!inTime.empty()) {
            cout << "入库时间: " << inTime << endl;
        }
        cout << "══════════════════════════════════════" << endl;
        
        // 确认添加
        if (getConfirmation("确认添加此图书？")) {
            // 创建图书对象
            BaseBook newBook(bid, bname, author, press, totalNum, borrowNum, inTime);
            
            // 调用Library的接口
            lib.mop.addBook(newBook);
            
            cout << "\n图书上架成功！" << endl;
            cout << "======================================" << endl;
        } else {
            cout << "操作已取消" << endl;
        }
        
    } catch (const exception& e) {
        cout << "上架失败: " << e.what() << endl;
    } catch (...) {
        cout << "上架过程中发生未知错误" << endl;
    }
}

// 增加库存
void addBookStock(Library& lib) {
    printTitle("增加图书库存");
    
    // 先显示现有图书
    int bookCount = lib.op.showBook();
    if (bookCount == 0) {
        cout << "当前没有图书，无法增加库存" << endl;
        return;
    }
    
    cout << "\n请选择要增加库存的图书：" << endl;
    cout << "1. 按序号选择" << endl;
    cout << "2. 按图书ID搜索" << endl;
    cout << "0. ↩返回" << endl;
    
    int choice = getIntegerInput("请选择: ", 0, 2);
    
    if (choice == 0) return;
    
    try {
        string targetBid;
        
        if (choice == 1) {
            // 按序号选择
            int num = getIntegerInput("请输入图书序号: ", 1, bookCount);
            targetBid = lib.op.showBook(num);
        } else if (choice == 2) {
            // 按ID搜索
            targetBid = getStringInput("请输入图书ID: ");
            if (!lib.op.bidExist(targetBid)) {
                cout << "图书ID不存在" << endl;
                return;
            }
        }
        
        if (!targetBid.empty()) {
            // 显示当前库存
            const BaseBook& book = lib.getBookList().getByBid(targetBid);
            cout << "\n当前图书信息：" << endl;
            cout << "══════════════════════════════" << endl;
            cout << "书名: " << book.getBname() << endl;
            cout << "当前库存: " << book.getTotalNum() << endl;
            cout << "已借出: " << book.getBorrowNum() << endl;
            cout << "══════════════════════════════" << endl;
            
            // 获取增加数量
            int addNum = getIntegerInput("请输入要增加的数量: ", 1, 1000);
            
            // 确认增加
            if (getConfirmation("确认增加库存？")) {
                lib.mop.addBook(targetBid, addNum);
                cout << "库存增加成功！" << endl;
                cout << "新库存: " << (book.getTotalNum() + addNum) << " 册" << endl;
            } else {
                cout << "操作已取消" << endl;
            }
        }
        
    } catch (const exception& e) {
        cout << "增加库存失败: " << e.what() << endl;
    }
}

// 上架主函数
void addBookManager(Library& lib) {
    while (true) {
        system("cls || clear");  // 清屏
        showAddBookMenu();
        
        int choice = getIntegerInput("请选择操作: ", 0, 2);
        
        switch (choice) {
            case 1:  // 新增图书
                addNewBook(lib);
                pressAnyKeyToContinue();
                break;
                
            case 2:  // 增加库存
                addBookStock(lib);
                pressAnyKeyToContinue();
                break;
                
            case 0:  // 返回
                return;
                
            default:
                cout << "无效选择" << endl;
                pressAnyKeyToContinue();
        }
    }
}

//图书下架

// 显示下架菜单
void showDeleteBookMenu() {
    printTitle("图书下架");
    cout << "请选择下架方式：" << endl;
    cout << "1. 按序号下架" << endl;
    cout << "2. 按图书ID下架" << endl;
    cout << "3. 查看可删除图书列表" << endl;
    cout << "0. 返回主菜单" << endl;
    cout << "══════════════════════════════════════════════════════════════════════" << endl;
}

// 查看可删除图书列表
void showDeletableBooks(Library& lib) {
    printTitle("可删除图书列表");
    
    int bookCount = lib.op.showBook();
    if (bookCount == 0) {
        cout << "当前没有图书" << endl;
        return;
    }
    
    cout << "\n图书列表（标记可删除状态）" << endl;
    cout << "════════════════════════════════════════════════════════════════" << endl;
    cout << "序号  图书ID      书名                作者      库存  借出  状态" << endl;
    cout << "════════════════════════════════════════════════════════════════" << endl;
    
    int deletableCount = 0;
    for (int i = 1; i <= bookCount; i++) {
        try {
            string bid = lib.op.showBook(i);
            const BaseBook& book = lib.getBookList().getByBid(bid);
            
            bool canDelete = lib.mop.canDelb(bid);
            
            // 格式化输出
            cout << setw(4) << i << ". "
                 << setw(10) << left << (book.getBid().length() > 8 ? book.getBid().substr(0, 6) + ".." : book.getBid())
                 << setw(20) << left << (book.getBname().length() > 18 ? book.getBname().substr(0, 15) + "..." : book.getBname())
                 << setw(8) << left << (book.getAuthor().length() > 6 ? book.getAuthor().substr(0, 4) + ".." : book.getAuthor())
                 << setw(6) << right << book.getTotalNum()
                 << setw(6) << right << book.getBorrowNum()
                 << "  ";
            
            if (canDelete) {
                cout << "可删除";
                deletableCount++;
            } else {
                cout << "不可删（有借出）";
            }
            cout << endl;
            
        } catch (...) {
            // 忽略错误
        }
    }
    
    cout << "════════════════════════════════════════════════════════════════" << endl;
    cout << "统计：共 " << bookCount << " 本图书，其中 " << deletableCount << " 本可删除" << endl;
}

// 按序号下架
void deleteBookByIndex(Library& lib) {
    printTitle("按序号下架图书");
    
    int bookCount = lib.op.showBook();
    if (bookCount == 0) {
        cout << "当前没有图书" << endl;
        return;
    }
    
    // 显示可删除状态
    showDeletableBooks(lib);
    cout << endl;
    
    int num = getIntegerInput("请输入要下架的图书序号: ", 1, bookCount);
    
    try {
        // 检查是否可删除
        if (!lib.mop.canDelb(num)) {
            cout << "该图书不可删除，原因：" << endl;
            string bid = lib.op.showBook(num);
            const BaseBook& book = lib.getBookList().getByBid(bid);
            cout << "   当前有 " << book.getBorrowNum() << " 本被借出" << endl;
            cout << "   请确保所有图书都已归还后再下架" << endl;
            return;
        }
        
        // 获取图书ID
        string bid = lib.op.showBook(num);
        
        // 显示要删除的图书信息
        const BaseBook& book = lib.getBookList().getByBid(bid);
        cout << "\n将要删除的图书：" << endl;
        cout << "══════════════════════════════" << endl;
        book.show();
        cout << "══════════════════════════════" << endl;
        
        // 确认删除
        if (getConfirmation("确认删除此图书？")) {
            lib.mop.delBook(bid);
            cout << "图书下架成功！" << endl;
        } else {
            cout << "操作已取消" << endl;
        }
        
    } catch (const exception& e) {
        cout << "下架失败: " << e.what() << endl;
    }
}

// 按ID下架
void deleteBookById(Library& lib) {
    printTitle("按图书ID下架");
    
    int bookCount = lib.op.showBook();
    if (bookCount == 0) {
        cout << "📭 当前没有图书" << endl;
        return;
    }
    
    // 显示现有图书
    cout << "现有图书列表：" << endl;
    lib.op.showBook();
    cout << endl;
    
    string bid = getStringInput("请输入要下架的图书ID: ");
    
    // 检查图书是否存在
    if (!lib.op.bidExist(bid)) {
        cout << "图书ID不存在" << endl;
        return;
    }
    
    try {
        // 检查是否可删除
        if (!lib.mop.canDelb(bid)) {
            cout << "该图书不可删除，有借出记录" << endl;
            return;
        }
        
        // 显示要删除的图书信息
        const BaseBook& book = lib.getBookList().getByBid(bid);
        cout << "\n将要删除的图书：" << endl;
        cout << "══════════════════════════════" << endl;
        book.show();
        cout << "══════════════════════════════" << endl;
        
        // 确认删除
        if (getConfirmation("确认删除此图书？")) {
            lib.mop.delBook(bid);
            cout << "图书下架成功！" << endl;
        } else {
            cout << "操作已取消" << endl;
        }
        
    } catch (const exception& e) {
        cout << "下架失败: " << e.what() << endl;
    }
}

// 下架主函数
void deleteBookManager(Library& lib) {
    while (true) {
        system("cls || clear");  // 清屏
        showDeleteBookMenu();
        
        int choice = getIntegerInput("请选择操作: ", 0, 3);
        
        switch (choice) {
            case 1:  // 按序号下架
                deleteBookByIndex(lib);
                pressAnyKeyToContinue();
                break;
                
            case 2:  // 按ID下架
                deleteBookById(lib);
                pressAnyKeyToContinue();
                break;
                
            case 3:  // 查看可删除列表
                showDeletableBooks(lib);
                pressAnyKeyToContinue();
                break;
                
            case 0:  // 返回
                return;
                
            default:
                cout << "无效选择" << endl;
                pressAnyKeyToContinue();
        }
    }
}

// ==================== 修改图书信息功能 ====================

// 显示修改菜单
void showUpdateBookMenu() {
    printTitle("修改图书信息");
    cout << "请选择查找方式：" << endl;
    cout << "1. 按序号查找" << endl;
    cout << "2. 按图书ID查找" << endl;
    cout << "3. 搜索图书" << endl;
    cout << "0. 返回主菜单" << endl;
    cout << "══════════════════════════════════════════════════════════════════════" << endl;
}

// 修改图书信息
void updateBookInfo(Library& lib, const string& bid) {
    try {
        // 获取原始图书
        BaseBook& originalBook = lib.getBookList().getByBid(bid);
        
        printTitle("修改图书信息");
        cout << "当前图书信息：" << endl;
        cout << "════════════════════════════════════════════" << endl;
        originalBook.show();
        cout << "════════════════════════════════════════════" << endl;
        
        // 创建修改副本
        BaseBook updatedBook = originalBook;
        
        cout << "\n请输入新的图书信息（留空保持原值）" << endl;
        cout << "════════════════════════════════════════════" << endl;
        
        // 修改书名
        string newBname = getStringInput("书名 [" + originalBook.getBname() + "]: ", true);
        if (!newBname.empty()) {
            updatedBook.setBname(newBname);
        }
        
        // 修改作者
        string newAuthor = getStringInput("作者 [" + originalBook.getAuthor() + "]: ", true);
        if (!newAuthor.empty()) {
            updatedBook.setAuthor(newAuthor);
        }
        
        // 修改出版社
        string newPress = getStringInput("出版社 [" + originalBook.getPress() + "]: ", true);
        if (!newPress.empty()) {
            updatedBook.setPress(newPress);
        }
        
        // 修改总数量
        cout << "总数量 [" << originalBook.getTotalNum() << "]: ";
        string totalNumStr;
        getline(cin, totalNumStr);
        if (!totalNumStr.empty()) {
            try {
                int newTotal = stoi(totalNumStr);
                if (newTotal >= originalBook.getBorrowNum()) {
                    updatedBook.setTotalNum(newTotal);
                } else {
                    cout << "总数量不能少于已借出数量，保持原值" << endl;
                }
            } catch (...) {
                cout << "输入无效，保持原值" << endl;
            }
        }
        
        // 修改借出数量
        cout << "已借出数量 [" << originalBook.getBorrowNum() << "]: ";
        string borrowNumStr;
        getline(cin, borrowNumStr);
        if (!borrowNumStr.empty()) {
            try {
                int newBorrow = stoi(borrowNumStr);
                if (newBorrow <= updatedBook.getTotalNum()) {
                    updatedBook.setBorrowNum(newBorrow);
                } else {
                    cout << "借出数量不能超过总数量，保持原值" << endl;
                }
            } catch (...) {
                cout << "输入无效，保持原值" << endl;
            }
        }
        
        // 修改入库时间
        string newInTime = getStringInput("入库时间 [" + originalBook.getInTime() + "]: ", true);
        if (!newInTime.empty()) {
            updatedBook.setInTime(newInTime);
        }
        
        // 显示修改后的信息
        cout << "\n修改后的图书信息：" << endl;
        cout << "════════════════════════════════════════════" << endl;
        updatedBook.show();
        cout << "════════════════════════════════════════════" << endl;
        
        // 确认修改
        if (getConfirmation("确认保存修改？")) {
            lib.getBookList().update(updatedBook);
            cout << "图书信息修改成功！" << endl;
        } else {
            cout << "修改已取消" << endl;
        }
        
    } catch (const exception& e) {
        cout << "修改失败: " << e.what() << endl;
    }
}

// 按序号修改
void updateBookByIndex(Library& lib) {
    printTitle("按序号修改图书");
    
    int bookCount = lib.op.showBook();
    if (bookCount == 0) {
        cout << "当前没有图书" << endl;
        return;
    }
    
    int num = getIntegerInput("请输入要修改的图书序号: ", 1, bookCount);
    
    try {
        string bid = lib.op.showBook(num);
        updateBookInfo(lib, bid);
    } catch (...) {
        cout << "获取图书失败" << endl;
    }
}

// 按ID修改
void updateBookById(Library& lib) {
    printTitle("按图书ID修改");
    
    int bookCount = lib.op.showBook();
    if (bookCount == 0) {
        cout << "📭 当前没有图书" << endl;
        return;
    }
    
    // 显示现有图书
    cout << "现有图书列表：" << endl;
    lib.op.showBook();
    cout << endl;
    
    string bid = getStringInput("请输入要修改的图书ID: ");
    
    if (!lib.op.bidExist(bid)) {
        cout << "图书ID不存在" << endl;
        return;
    }
    
    updateBookInfo(lib, bid);
}

// 搜索并修改
void searchAndUpdateBook(Library& lib) {
    printTitle("搜索并修改图书");
    
    int bookCount = lib.op.showBook();
    if (bookCount == 0) {
        cout << "当前没有图书" << endl;
        return;
    }
    
    string keyword = getStringInput("请输入搜索关键词: ");
    
    cout << "\n搜索结果：" << endl;
    lib.op.schBook(keyword);
    cout << endl;
    
    if (bookCount > 0) {
        cout << "请选择操作：" << endl;
        cout << "1. 按搜索结果中的序号修改" << endl;
        cout << "2. 输入图书ID修改" << endl;
        cout << "0. 返回" << endl;
        
        int choice = getIntegerInput("请选择: ", 0, 2);
        
        if (choice == 1) {
            int num = getIntegerInput("请输入搜索结果中的序号: ", 1, bookCount);
            // 这里需要从搜索结果中获取实际图书ID
            // 简化：调用原始showBook
            try {
                string bid = lib.op.showBook(num);
                updateBookInfo(lib, bid);
            } catch (...) {
                cout << "获取图书失败" << endl;
            }
        } else if (choice == 2) {
            string bid = getStringInput("请输入图书ID: ");
            if (lib.op.bidExist(bid)) {
                updateBookInfo(lib, bid);
            } else {
                cout << "图书ID不存在" << endl;
            }
        }
    }
}

// 修改主函数
void updateBookManager(Library& lib) {
    while (true) {
        system("cls || clear");  // 清屏
        showUpdateBookMenu();
        
        int choice = getIntegerInput("请选择操作: ", 0, 3);
        
        switch (choice) {
            case 1:  // 按序号修改
                updateBookByIndex(lib);
                pressAnyKeyToContinue();
                break;
                
            case 2:  // 按ID修改
                updateBookById(lib);
                pressAnyKeyToContinue();
                break;
                
            case 3:  // 搜索并修改
                searchAndUpdateBook(lib);
                pressAnyKeyToContinue();
                break;
                
            case 0:  // 返回
                return;
                
            default:
                cout << "无效选择" << endl;
                pressAnyKeyToContinue();
        }
    }
}

// ==================== 图书管理主菜单 ====================

// 显示图书管理主菜单
void showBookManagerMenu() {
    system("cls || clear");  // 清屏
    
    printTitle("图书管理模块");
    cout << "请选择要执行的操作：" << endl;
    cout << "1. 图书上架（新增/增加库存）" << endl;
    cout << "2. 图书下架（删除图书）" << endl;
    cout << "3. 修改图书信息" << endl;
    cout << "4. 查看所有图书" << endl;
    cout << "5. 搜索图书" << endl;
    cout << "6. 查看图书详情" << endl;
    cout << "7. 按入库时间排序" << endl;
    cout << "0. 返回主菜单" << endl;
    cout << "══════════════════════════════════════════════════════════════════════" << endl;
}

// 查看图书详情
void viewBookDetail(Library& lib) {
    printTitle("查看图书详情");
    
    int bookCount = lib.op.showBook();
    if (bookCount == 0) {
        cout << "📭 当前没有图书" << endl;
        return;
    }
    
    cout << "请选择查找方式：" << endl;
    cout << "1. 按序号查看" << endl;
    cout << "2. 按图书ID查看" << endl;
    cout << "0. 返回" << endl;
    
    int choice = getIntegerInput("请选择: ", 0, 2);
    
    if (choice == 1) {
        int num = getIntegerInput("请输入图书序号: ", 1, bookCount);
        try {
            string bid = lib.op.showBook(num);
            lib.op.showBook(bid);
        } catch (...) {
            cout << "获取图书失败" << endl;
        }
    } else if (choice == 2) {
        string bid = getStringInput("请输入图书ID: ");
        if (lib.op.bidExist(bid)) {
            lib.op.showBook(bid);
        } else {
            cout << "图书ID不存在" << endl;
        }
    }
}

// 按入库时间排序
void sortBooksByTime(Library& lib) {
    printTitle("按入库时间排序");
    
    cout << "请选择排序方式：" << endl;
    cout << "1. 升序排序（从早到晚）" << endl;
    cout << "2. 降序排序（从晚到早）" << endl;
    cout << "0. 返回" << endl;
    
    int choice = getIntegerInput("请选择: ", 0, 2);
    
    if (choice == 1 || choice == 2) {
        lib.getBookList().sortByInTime(choice == 1);
        cout << "排序完成！" << endl;
        cout << "排序后的图书列表：" << endl;
        lib.op.showBook();
    }
}

// 图书管理主函数
void bookManagerMain(Library& lib) {
    while (true) {
        showBookManagerMenu();
        
        int choice = getIntegerInput("请选择操作: ", 0, 7);
        
        switch (choice) {
            case 1:  // 上架
                addBookManager(lib);
                break;
                
            case 2:  // 下架
                deleteBookManager(lib);
                break;
                
            case 3:  // 修改
                updateBookManager(lib);
                break;
                
            case 4:  // 查看所有
                system("cls || clear");
                printTitle("所有图书列表");
                lib.op.showBook();
                pressAnyKeyToContinue();
                break;
                
            case 5:  // 搜索
                {
                    system("cls || clear");
                    printTitle("搜索图书");
                    string keyword = getStringInput("请输入搜索关键词: ");
                    lib.op.schBook(keyword);
                    pressAnyKeyToContinue();
                }
                break;
                
            case 6:  // 查看详情
                viewBookDetail(lib);
                pressAnyKeyToContinue();
                break;
                
            case 7:  // 排序
                sortBooksByTime(lib);
                pressAnyKeyToContinue();
                break;
                
            case 0:  // 返回
                return;
                
            default:
                cout << "无效选择" << endl;
                pressAnyKeyToContinue();
        }
    }
}

// ==================== 主入口函数 ====================

// 测试函数
void testBookManager() {
    cout << "正在初始化图书馆系统..." << endl;
    
    // 创建Library对象
    Library lib;
    
    // 导入示例数据（如果为空）
    if (lib.op.showBook() == 0) {
        cout << "未找到图书数据，导入示例图书..." << endl;
        
        // 添加示例图书
        BaseBook book1("B001", "C++ Primer", "Stanley Lippman", 
                      "机械工业出版社", 10, 3);
        BaseBook book2("B002", "算法导论", "Thomas Cormen", 
                      "机械工业出版社", 8, 2);
        BaseBook book3("B003", "深入理解计算机系统", "Randal Bryant", 
                      "机械工业出版社", 5, 0);
        
        lib.mop.addBook(book1);
        lib.mop.addBook(book2);
        lib.mop.addBook(book3);
        
        cout << "示例图书导入完成" << endl;
    }
    
    // 运行图书管理系统
    bookManagerMain(lib);
    
    cout << "感谢使用图书管理系统！" << endl;
}

// 主函数（供队友集成）
int main() {
    testBookManager();
    return 0;
}