//
// Created by LesSugarR on 25-12-10.
//
//

#include "../include/Book.h"
#include "../include/History.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <ctime>
#include <algorithm>
#include <sstream>

using namespace std;
using namespace LibraryBookSystem;

// 时间比较函数
bool compareInTimeAsc(const BaseBook &a, const BaseBook &b) {
    return a.getInTime() < b.getInTime();
}
bool compareInTimeDesc(const BaseBook &a, const BaseBook &b) {
    return a.getInTime() > b.getInTime();
}

// BaseBook 成员函数实现
bool BaseBook::match(const string &s) const {
    bool isBidMatch = (this->getBid() == s);
    bool isNameMatch = (this->getBname() == s);
    bool isAuthorMatch = (this->getAuthor() == s);
    bool isPressMatch = (this->getPress() == s);
    bool isTimeMatch = (this->getInTime() == s);
    return isBidMatch || isNameMatch || isAuthorMatch || isPressMatch || isTimeMatch;
}

void BaseBook::show() const {
    cout << this->getBid() << "\t" << this->getBname() << "\t" << this->getAuthor()
         << "\t" << this->getPress() << "\t" << this->getTotalNum()
         << "\t" << this->getBorrowNum() << "\t" << this->getInTime() << endl;
}

// 流读取参数数量匹配（适配实际输入，解决“参数太少”）
istream &operator>>(istream &in, BaseBook &bb) {
    string tempBid, tempName, tempAuthor, tempPress, tempInTime;
    int tempTotal, tempBorrow;

    // 按“ID 名称 作者 出版社 总数 借出数 时间”完整读取（确保参数数量）
    in >> tempBid >> tempName >> tempAuthor >> tempPress >> tempTotal >> tempBorrow >> tempInTime;

    // 数据合法性校验
    if (tempBid.empty()) throw runtime_error("书籍ID不能为空！");
    if (tempTotal < 0 || tempBorrow < 0) throw runtime_error("书籍数量不能为负数！");
    if (tempBorrow > tempTotal) throw runtime_error("借出数量不能超过总数量！");

    // Setter赋值
    bb.setBid(tempBid);
    bb.setBname(tempName);
    bb.setAuthor(tempAuthor);
    bb.setPress(tempPress);
    bb.setTotalNum(tempTotal);
    bb.setBorrowNum(tempBorrow);
    bb.setInTime(tempInTime.empty() ? BaseBook::getCurrentTime() : tempInTime);

    return in;
}

ostream &operator<<(ostream &out, const BaseBook &bb) {
    out << bb.getBid() << endl
        << bb.getBname() << endl
        << bb.getAuthor() << endl
        << bb.getPress() << endl
        << bb.getTotalNum() << endl
        << bb.getBorrowNum() << endl
        << bb.getInTime() << endl;
    return out;
}

// Book 成员函数实现
void Book::addNum(const int &num) {
    if (num < 0) throw runtime_error("添加数量不能为负数！");
    this->setTotalNum(this->getTotalNum() + num);
}

void Book::reqBorrow(const ::BaseHistory &bh) {
    this->borrowReq.add(bh);
}

//bool表达式正常（基类canBorrow()返回bool，无冲突）
void Book::agrBorrow(const ::BaseHistory &bh) {
    // 正确识别基类的bool canBorrow()
    if (!this->BaseBook::canBorrow()) { 
        throw runtime_error("书籍库存不足，无法借阅！");
    }
    this->setBorrowNum(this->getBorrowNum() + 1);
    this->borrowHis.add(bh);
}

// 函数名改为cancelBorrow，避免与基类冲突
void Book::cancelBorrow(const ::BaseHistory &bh) {
    this->returnHis.add(bh);
}

void Book::refBorrow(const ::BaseHistory &bh) {
    this->returnHis.add(bh);
}

void Book::reqReturn(const ::BaseHistory &bh) {
    this->returnReq.add(bh);
}

void Book::agrReturn(const ::BaseHistory &bh) {
    if (this->getBorrowNum() - 1 < 0) throw runtime_error("借出数量异常，无法还书！");
    this->setBorrowNum(this->getBorrowNum() - 1);
    this->returnHis.add(bh);
}

void Book::canReturn(const ::BaseHistory &bh) {
    this->borrowHis.add(bh);
}

void Book::refReturn(const ::BaseHistory &bh) {
    this->borrowHis.add(bh);
}

int Book::showBorrowReq() const {
    return this->borrowReq.size();
}

int Book::showBorrowHis() const {
    return this->borrowHis.size();
}

int Book::showReturnReq() const {
    return this->returnReq.size();
}

int Book::showReturnHis() const {
    return this->returnHis.size();
}

string Book::showBorrowReq(const int &num) const {
    return "BorrowReq_" + to_string(num);
}

string Book::showBorrowHis(const int &num) const {
    return "BorrowHis_" + to_string(num);
}

string Book::showReturnReq(const int &num) const {
    return "ReturnReq_" + to_string(num);
}

string Book::showReturnHis(const int &num) const {
    return "ReturnHis_" + to_string(num);
}

// Book流提取：参数读取原本这处有问题，后修改
istream &operator>>(istream &in, Book &b) {
    string tempBid, tempName, tempAuthor, tempPress, tempInTime;
    int tempTotal, tempBorrow;

    in >> tempBid >> tempName >> tempAuthor >> tempPress >> tempTotal >> tempBorrow >> tempInTime;

    if (tempBid.empty()) throw runtime_error("书籍ID不能为空！");
    if (tempTotal < 0 || tempBorrow < 0 || tempBorrow > tempTotal) {
        throw runtime_error("书籍数量输入异常！");
    }

    b.setBid(tempBid);
    b.setBname(tempName);
    b.setAuthor(tempAuthor);
    b.setPress(tempPress);
    b.setTotalNum(tempTotal);
    b.setBorrowNum(tempBorrow);
    b.setInTime(tempInTime);

    return in;
}

ostream &operator<<(ostream &out, const Book &b) {
    out << b.getBid() << endl
        << b.getBname() << endl
        << b.getAuthor() << endl
        << b.getPress() << endl
        << b.getTotalNum() << endl
        << b.getBorrowNum() << endl
        << b.getInTime() << endl;
    return out;
}

//  BookList 成员函数实现
bool BookList::bidExist(const string &bid) const {
    for (auto rit = this->bl.rbegin(); rit != this->bl.rend(); ++rit) {
        if (rit->getBid() == bid) {
            return true;
        }
    }
    return false;
}

void BookList::add(const BaseBook &bb) {
    if (this->bidExist(bb.getBid())) {
        throw runtime_error("书籍ID已存在，无法重复添加！");
    }
    this->bl.push_back(bb);
}

void BookList::del(const BaseBook &bb) {
    this->del(bb.getBid());
}

void BookList::del(const string &bid) {
    if (!this->bidExist(bid)) {
        throw runtime_error("书籍ID不存在，无法删除！");
    }
    for (auto it = this->bl.begin(); it != this->bl.end(); ++it) {
        if (it->getBid() == bid) {
            if (!it->canDel()) throw runtime_error("书籍仍有借出，无法删除！");
            this->bl.erase(it);
            return;
        }
    }
}

int BookList::show() const {
    cout << "编号\t书籍id\t书籍名称\t作者\t出版社\t书籍总数\t借出数量\t入库时间" << endl;
    int count = 0;
    for (auto it = this->bl.begin(); it != this->bl.end(); ++it) {
        cout << ++count << ".\t";
        it->show();
    }
    return this->bl.size();
}

string BookList::show(const int &num) const {
    if (num < 0 || num >= (int)this->bl.size()) {
        throw out_of_range("索引越界！");
    }
    this->bl[num].show();
    return this->bl[num].getBid();
}

void BookList::show(const string &bid) const {
    for (auto it = this->bl.begin(); it != this->bl.end(); ++it) {
        if (it->getBid() == bid) {
            it->show();
            return;
        }
    }
    throw runtime_error("书籍ID不存在！");
}

void BookList::schBook(const string &s) const {
    cout << "编号\t书籍id\t书籍名称\t作者\t出版社\t书籍总数\t借出数量\t入库时间" << endl;
    int cnt = 0;
    for (auto it = this->bl.begin(); it != this->bl.end(); ++it) {
        if (it->match(s)) {
            cout << ++cnt << ".\t";
            it->show();
        }
    }
    if (cnt == 0) {
        cout << "未找到匹配书籍！" << endl;
    }
}

void BookList::sortByInTime(bool asc) {
    if (asc) {
        sort(this->bl.begin(), this->bl.end(), compareInTimeAsc);
    } else {
        sort(this->bl.begin(), this->bl.end(), compareInTimeDesc);
    }
    cout << "图书已按入库时间" << (asc ? "升序" : "降序") << "排序完成！" << endl;
}

// 运算符>>匹配（参数类型一致）
istream &operator>>(istream &in, BookList &booklist) {
    int bookCount;
    in >> bookCount;
    in.ignore();
    for (int i = 0; i < bookCount; i++) {
        BaseBook bb;
        LibraryBookSystem::operator>>(in,bb); // 匹配BaseBook的>>运算符
        booklist.add(bb);
    }
    return in;
}

// []运算符
ostream &operator<<(ostream &out, const BookList &booklist) {
    out << booklist.size() << endl;
    for (int i=0;i<booklist.size();i++) {
        LibraryBookSystem::operator<<(out,booklist.getBookAt(i)); 
    }
    return out;
}

std::istream& operator>>(std::istream& in,LibraryBookSystem::BaseBook& bb){
    return LibraryBookSystem::operator>>(in,bb);
}
std::ostream& operator>>(std::ostream& out,const LibraryBookSystem::BaseBook& bb){
    return LibraryBookSystem::operator<<(out,bb);
}