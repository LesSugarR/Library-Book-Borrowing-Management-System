#ifndef BOOK_H
#define BOOK_H

#include "History.h"
#include <bits/stdc++.h>
#define BH BaseHistory
#define HL HistoryList
using namespace std;

//Book.h
class BaseBook{
protected:
    string bid, bname, author, press;
    int totalNum, borrowNum;
    string category; // 新增：对应前端的分类
    string status;   // 新增：对应前端的状态
public:
    BaseBook(string _bid, string _bname, string _author, string _press, int _totalNum, string _cat="计算机")
            : bid(_bid), bname(_bname), author(_author), press(_press), totalNum(_totalNum), borrowNum(0), category(_cat), status("可借") {}
    BaseBook() {}
    virtual ~BaseBook() {}

    string getBid() const { return bid; }
    string getBname() const { return bname; }
    string getAuthor()const { return author; }
    string getPress()const { return press; }
    string getCategory()const { return category; }
    // 简单的 JSON 输出
    virtual string toJson() const;

    friend istream &operator >> (istream &in, BaseBook &bb);
    friend ostream &operator << (ostream &out, const BaseBook &bb);
    //其他功能
    bool match(const string &s) const;
    void show() const;
};

class Book : public BaseBook{
    HL borrowReq;
    HL borrowHis;
    HL returnReq;
    HL returnHis;
public:
    using BaseBook::BaseBook;
    Book(const Book& other);
    Book& operator=(const Book& other);
    Book() : BaseBook() {}
    Book(const BaseBook& bb) : BaseBook(bb) {}
    BaseBook getBase() const
    {
        return *this;
    }
    void addNum(const int &num);
    void reqBorrow(const BH &bh);
    void agrBorrow(const BH &bh);
    void refBorrow(const BH &bh);
    bool canBorrow() const;
    void canBorrow(const BH &bh);
    void reqReturn(const BH &bh);
    void agrReturn(const BH &bh);
    void canReturn(const BH &bh);
    void refReturn(const BH &bh);
    bool canDel() const;
    int showBorrowReq() const;
    int showBorrowHis() const;
    int showReturnReq() const;
    int showReturnHis() const;		//将Reader的相关历史记录输出到屏幕上并返回相应历史记录的数量
    string toJson() const override;
    friend ostream &operator << (ostream &out, const Book &b);
    friend istream &operator >> (istream &in, Book &b);
};

// BookList 类的声明
class BookList {
private:
    // 链表节点结构
    struct BookNode {
        Book data;
        BookNode* next;
        BookNode(const Book& book) : data(book), next(nullptr) {}
    };

    BookNode* head;
    BookNode* tail;
    int count;

    // 私有辅助函数
    BookNode* findNode(const string& bid) const;
    BookNode* findNode(int index) const;
    void clear();

public:
    BookList();
    ~BookList();
    BookList(const BookList& other);
    BookList& operator=(const BookList& other);
    string toJsonString() const;
    Book& getByBid(const string& bid);
    Book &getByIndex(const int &index);
    int size() const { return count; }
    bool bidExist(const string& bid) const;
    void add(const Book &b);
    void del(const BaseBook &bb);
    void del(const string &bid);
    Book& operator[](const string &bid);
    Book& operator[](const int &index);
    int show() const;
    string show(const int &num) const;
    void show(const string &bid) const;
    void schBook(const string &s) const;
    void saveToJsonFile(const string& filename) const;//保存为 JSON 文件供 Python 读取
    friend istream &operator >> (istream &in, BookList &bl);
    friend ostream &operator << (ostream &out, const BookList &bl);
    string getBname(const string &bid);
};

#endif