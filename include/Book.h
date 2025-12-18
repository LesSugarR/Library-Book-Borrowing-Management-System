// Created by LesSugarR on 25-12-10.
//
//明确图书要存哪些信息（定义数据结构）


#ifndef BOOK_H
#define BOOK_H

#include <string>

namespace LibraryBookSystem {

// 基础图书结构体
struct BaseBook {
protected:
    std::string bid;
    std::string bname;
    std::string author;
    std::string press;
    int totalNum;
    int borrowNum;
    std::string inTime;  

public:
    // 构造函数
    BaseBook(std::string _bid = "", std::string _bname = "", 
             std::string _author = "", std::string _press = "",
             int _totalNum = 0, int _borrowNum = 0, 
             std::string _inTime = "");

    // 获取当前时间
    static std::string getCurrentTime();

    // Getter函数
    std::string getBid() const;
    std::string getBname() const;
    std::string getAuthor() const;
    std::string getPress() const;
    int getTotalNum() const;
    int getBorrowNum() const;
    std::string getInTime() const;

    // Setter函数
    void setBid(const std::string& id);
    void setBname(const std::string& name);
    void setAuthor(const std::string& auth);
    void setPress(const std::string& p);
    void setTotalNum(int num);
    void setBorrowNum(int num);
    void setInTime(const std::string& time);

    // 判断函数
    bool canBorrow() const;
    bool canDelete() const;

    // 匹配函数
    bool match(const std::string& keyword) const;

    // 显示图书信息
    void show() const;
}; 

// 图书链表节点
struct BookNode {
    BaseBook data;
    BookNode* next;
    
    BookNode(const BaseBook& book);
    BookNode();
};

// 图书链表管理类
class BookList {
private:
    BookNode* head;
    BookNode* tail;
    int size;

    // 辅助函数（私有）
    BookNode* getNodeAt(int index) const;
    BookNode* findPrevByBid(const std::string& bid) const;
    BookNode* getNodeByBid(const std::string& bid) const;
    void clear();

public:
    // 构造函数和析构函数
    BookList();
    ~BookList();

    // 基本操作
    int getSize() const;
    bool bidExists(const std::string& bid) const;
    void add(const BaseBook& book);
    void remove(const std::string& bid);
    void remove(const BaseBook& book);
    void update(const BaseBook& book);
    
    // 查询操作
    BaseBook& getByBid(const std::string& bid);
    const BaseBook& getByBid(const std::string& bid) const;
    BaseBook& getByIndex(int index);
    const BaseBook& getByIndex(int index) const;
    
    // 显示和搜索
    void showAll() const;
    void search(const std::string& keyword) const;
    
    // 排序
    void sortByInTime(bool ascending = true);
    
    // 运算符重载
    BaseBook& operator[](const std::string& bid);
    BaseBook& operator[](int index);
    const BaseBook& operator[](const std::string& bid) const;
    const BaseBook& operator[](int index) const;
};

// 输入输出运算符声明
std::istream& operator>>(std::istream& in, BaseBook& book);
std::ostream& operator<<(std::ostream& out, const BaseBook& book);

} // namespace LibraryBookSystem

#endif // BOOK_H