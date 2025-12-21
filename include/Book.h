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
    string bid,bname,author,press;
    int totalNum,borrowNum;
public:
    BaseBook(string _bid, string _bname,string _author,string _press,int _totalNum,int _borrowNum=0)
            :bid(_bid),bname(_bname),author(_author),press(_press),totalNum(_totalNum),borrowNum(_borrowNum){}
    BaseBook(){}
    string getBid() const { return bid; }
    string getBname() const { return bname; }
    string getAuthor() const { return author; }
    string getPress() const { return press; }
    int getBorrowNum() const { return borrowNum; }
    int getTotalNum() const { return totalNum; }
    bool canBorrow() const { return totalNum-borrowNum>0; }
    bool canDel() const { return borrowNum==0; }
    bool match(const string &s) const;
    void show() const;		//将BaseBook对象存储的信息输出到屏幕上
    friend istream &operator >> (istream &in,BaseBook &bb);
    friend ostream &operator << (ostream &out,const BaseBook &bb);
};

class Book : public BaseBook{
    HL borrowReq;
    HL borrowHis;
    HL returnReq;
    HL returnHis;
public:
    using BaseBook::BaseBook;
    BaseBook getBase() const { return BaseBook(bid,bname,author,press,totalNum,borrowNum); }
    void addNum(const int &num);
    void reqBorrow(const BH &bh);
    void agrBorrow(const BH &bh);
    void canBorrow(const BH &bh);
    void refBorrow(const BH &bh);
    void reqReturn(const BH &bh);
    void agrReturn(const BH &bh);
    void canReturn(const BH &bh);
    void refReturn(const BH &bh);
    int showBorrowReq() const;
    int showBorrowHis() const;
    int showReturnReq() const;
    int showReturnHis() const;		//将Reader的相关历史记录输出到屏幕上并返回相应历史记录的数量
    friend ostream &operator << (ostream &out,const Book &b);
};

// BookList 类的声明
class BookList {
private:
    // 链表节点结构
    struct BookNode {
        BaseBook data;
        BookNode* next;

        BookNode(const BaseBook& book);
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

    BaseBook& getByBid(const string& bid);
    BaseBook& getByIndex(int index);
    int size() const { return count; }
    bool bidExist(const string& bid) const;
    void add(const BaseBook &bb);
    void del(const BaseBook &bb);
    void del(const string &bid);
    int show() const;
    string show(const int &num) const;
    void show(const string &bid) const;
    void schBook(const string &s) const;

    friend istream &operator >> (istream &in, BookList &bl);
    friend ostream &operator << (ostream &out, const BookList &bl);

    string getBname(const string &bid);
};

#endif