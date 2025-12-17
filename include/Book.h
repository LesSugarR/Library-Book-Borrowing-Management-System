// Created by LesSugarR on 25-12-10.
//
//明确图书要存哪些信息（定义数据结构）


#ifndef BOOK_H
#define BOOK_H

#include "History.h"
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <ctime>

// BaseHistory/HistoryList是全局类,::指定全局作用域
#define BH ::BaseHistory
#define HL ::HistoryList

namespace LibraryBookSystem {
// 1. BaseBook：基础数据结构
struct BaseBook {
protected:
    std::string bid, bname, author, press;
    int totalNum, borrowNum;
    std::string inTime;

public:
    // 带参构造函数
    BaseBook(std::string _bid, std::string _bname, std::string _author, std::string _press,
             int _totalNum, int _borrowNum = 0, std::string _inTime = "") {
        bid = _bid;
        bname = _bname;
        author = _author;
        press = _press;
        totalNum = _totalNum;
        borrowNum = _borrowNum;
        inTime = _inTime.empty() ? getCurrentTime() : _inTime;
    }

    // 无参构造函数
    BaseBook() {
        bid = "";
        bname = "";
        author = "";
        press = "";
        totalNum = 0;
        borrowNum = 0;
        inTime = getCurrentTime();
    }

    // 获取当前时间静态函数
    static std::string getCurrentTime() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buf[20];
        sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d",
                1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday,
                ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
        return std::string(buf);
    }

    // Getter函数
    std::string getInTime() const { return inTime; }
    std::string getBid() const { return bid; }
    std::string getBname() const { return bname; }
    std::string getAuthor() const { return author; }
    std::string getPress() const { return press; }
    int getBorrowNum() const { return borrowNum; }
    int getTotalNum() const { return totalNum; }

    // Setter函数
    void setBid(const std::string& _bid) { bid = _bid; }
    void setBname(const std::string& _bname) { bname = _bname; }
    void setAuthor(const std::string& _author) { author = _author; }
    void setPress(const std::string& _press) { press = _press; }
    void setTotalNum(int _totalNum) {
        if (_totalNum >= 0) totalNum = _totalNum; // 合法性校验
    }
    void setBorrowNum(int _borrowNum) {
        if (_borrowNum >= 0 && _borrowNum <= totalNum) borrowNum = _borrowNum; // 合法性校验
    }
    void setInTime(const std::string& _inTime) { inTime = _inTime.empty() ? getCurrentTime() : _inTime; }

    // 判断函数
    bool canBorrow() const { return (totalNum - borrowNum) > 0; }
    bool canDel() const { return borrowNum == 0; }

    // 成员函数声明
    bool match(const std::string& s) const;
    void show() const;

    // 友元运算符声明（作用域！！！）
    friend std::istream& operator>>(std::istream& in, BaseBook& book);
    friend std::ostream& operator<<(std::ostream& out, const BaseBook& book);
};

// 2. Book：继承BaseBook
struct Book : public BaseBook {
private:
    HL borrowReq;
    HL borrowHis;
    HL returnReq;
    HL returnHis;

public:
    // 继承基类构造函数
    using BaseBook::BaseBook;

    // 成员函数
    BaseBook getBase() const {
        return BaseBook(bid, bname, author, press, totalNum, borrowNum, inTime);
    }

    // 业务函数声明（全局BaseHistory）
    void addNum(const int& num);
    void reqBorrow(const ::BaseHistory& bh);
    void agrBorrow(const ::BaseHistory& bh);
    void cancelBorrow(const ::BaseHistory& bh);//取消借阅
    void refBorrow(const ::BaseHistory& bh);
    void reqReturn(const ::BaseHistory& bh);
    void agrReturn(const ::BaseHistory& bh);
    void canReturn(const ::BaseHistory& bh);
    void refReturn(const ::BaseHistory& bh);

    // 展示函数声明
    int showBorrowReq() const;
    int showBorrowHis() const;
    int showReturnReq() const;
    int showReturnHis() const;
    std::string showBorrowReq(const int& num) const;
    std::string showBorrowHis(const int& num) const;
    std::string showReturnReq(const int& num) const;
    std::string showReturnHis(const int& num) const;

    // 友元运算符重载声明
    friend std::istream& operator>>(std::istream& in, Book& book);
    friend std::ostream& operator<<(std::ostream& out, const Book& book);
};

// 3. BookList：书籍列表管理
struct BookList {
private:
    std::vector<BaseBook> bl;

public:
    // 重载[]运算符
    BaseBook& operator[](const std::string& bid) {
        for (size_t i = 0; i < bl.size(); ++i) {
            if (bl[i].getBid() == bid) {
                return bl[i];
            }
        }
        throw std::runtime_error("Bid does not exist!");
    }

    BaseBook& operator[](const int& num) {
        if (num < 0 || num >= (int)bl.size()) {
            throw std::out_of_range("Index out of bounds!");
        }
        return bl[num];
    }


    // 成员函数
    int size() const { return bl.size(); }

    //为了解决Book.cpp文件的多个<<运算符匹配报错问题，所以加了这个接口，通过公共接口获取元素，绕开bl
    const BaseBook& getBookAt(int index) const {
        if(index<0||index>=(int)bl.size()){
            throw std::out_of_range("Index out of bounds!");
        }
        return bl[index];
    }

    // 成员函数声明
    bool bidExist(const std::string& bid) const;
    void add(const BaseBook& bb);
    void del(const BaseBook& bb);
    void del(const std::string& bid);
    int show() const;
    std::string show(const int& num) const;
    void show(const std::string& bid) const;
    void schBook(const std::string& s) const;
    void sortByInTime(bool asc = true);

    // 友元运算符重载声明
    friend std::istream& operator>>(std::istream& in, BookList& booklist);
    friend std::ostream& operator<<(std::ostream& out, const BookList& booklist);
};
} // namespace LibraryBookSystem

using namespace LibraryBookSystem;
#endif // BOOK_H