//
// Created by LesSugarR on 25-12-10.
//

#include "Book.h"
#include "History.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#define BB LibraryBookSystem::BaseBook
#define BH LibraryBookSystem::BaseHistory
#define BL LibraryBookSystem::BookList
using namespace std;
using namespace LibraryBookSystem;

// BaseBook 成员函数实现
bool BB::match(const string &s) const {
   
    bool isBidMatch = (this->bid == s);
    bool isNameMatch = (this->bname == s);
    bool isAuthorMatch = (this->author == s);
    bool isPressMatch = (this->press == s);
    return isBidMatch || isNameMatch || isAuthorMatch || isPressMatch;
}

void BB::show() const {
   
    cout << this->bid << "\t" << this->bname << "\t" << this->author 
         << "\t" << this->press << "\t" << this->totalNum 
         << "\t" << this->borrowNum << endl;
}

istream &operator>>(istream &in, BB &bb) {
    // 分步读取数据，临时变量中转
    string tempBid, tempName, tempAuthor, tempPress;
    int tempTotal, tempBorrow;
    in >> tempBid >> tempName >> tempAuthor >> tempPress >> tempTotal >> tempBorrow;
    bb.bid = tempBid;
    bb.bname = tempName;
    bb.author = tempAuthor;
    bb.press = tempPress;
    bb.totalNum = tempTotal;
    bb.borrowNum = tempBorrow;
    return in;
}

ostream &operator<<(ostream &out, const BB &bb) {
    //用换行符拼接的逐行输出
    out << bb.bid << endl
        << bb.bname << endl
        << bb.author << endl
        << bb.press << endl
        << bb.totalNum << endl
        << bb.borrowNum << endl;
    return out;
}

// Book 成员函数实现
void Book::addNum(const int &num) {
    // 用临时变量中转
    int newTotal = this->totalNum + num;
    this->totalNum = newTotal;
}

void Book::reqBorrow(const BH &bh) {
    this->borrowReq.add(bh);
}

void Book::agrBorrow(const BH &bh) {
    
    this->borrowReq.del(bh);
    int newBorrow = this->borrowNum + 1;
    this->borrowHis.add(bh);
    this->borrowNum = newBorrow;
}

void Book::canBorrow(const BH &bh) {
    this->borrowReq.del(bh);
    this->returnHis.add(bh);
}

void Book::refBorrow(const BH &bh) {
    this->borrowReq.del(bh);
    this->returnHis.add(bh);
}

void Book::reqReturn(const BH &bh) {
    this->borrowHis.del(bh);
    this->returnReq.add(bh);
}

void Book::agrReturn(const BH &bh) {
    
    this->returnReq.del(bh);
    int newBorrow = this->borrowNum - 1;
    this->returnHis.add(bh);
    this->borrowNum = newBorrow;
}

void Book::canReturn(const BH &bh) {
    this->returnReq.del(bh);
    this->borrowHis.add(bh);
}

void Book::refReturn(const BH &bh) {
    this->returnReq.del(bh);
    this->borrowHis.add(bh);
}

int Book::showBorrowReq() const {
    return this->borrowReq.show();
}

int Book::showBorrowHis() const {
    return this->borrowHis.show();
}

int Book::showReturnReq() const {
    return this->returnReq.show();
}

int Book::showReturnHis() const {
    return this->returnHis.show();
}

string Book::showBorrowReq(const int &num) const {
    return this->borrowReq.show(num);
}

string Book::showBorrowHis(const int &num) const {
    return this->borrowHis.show(num);
}

string Book::showReturnReq(const int &num) const {
    return this->returnReq.show(num);
}

string Book::showReturnHis(const int &num) const {
    return this->returnHis.show(num);
}

istream &operator>>(istream &in, Book &b) {
    // 分步读取数据，临时变量中转
    string tempBid, tempName, tempAuthor, tempPress;
    int tempTotal, tempBorrow;
    in >> tempBid >> tempName >> tempAuthor >> tempPress >> tempTotal >> tempBorrow;
    b.bid = tempBid;
    b.bname = tempName;
    b.author = tempAuthor;
    b.press = tempPress;
    b.totalNum = tempTotal;
    b.borrowNum = tempBorrow;
    in >> b.borrowReq >> b.borrowHis >> b.returnReq >> b.returnHis;
    return in;
}

ostream &operator<<(ostream &out, const Book &b) {
    // 换行符拼接,逐行输出
    out << b.bid << endl
        << b.bname << endl
        << b.author << endl
        << b.press << endl
        << b.totalNum << endl
        << b.borrowNum << endl;
    out << b.borrowReq << b.borrowHis << b.returnReq << b.returnHis;
    return out;
}

// BookList 成员函数实现
BB &BL::operator[](const string &bid) {
    // 用迭代器遍历
    vector<BaseBook>::iterator it;
    for (it = this->bl.begin(); it != this->bl.end(); ++it) {
        if (it->getBid() == bid) {
            return *it;
        }
    }
    throw runtime_error("Bid does not exist!");
}

BB &BL::operator[](const int &num) {
    // 范围判断的临时变量
    bool isOutOfRange = (num < 0 || num >= (int)this->bl.size());
    if (isOutOfRange) {
        throw out_of_range("Index out of bounds!");
    }
    return this->bl[num];
}

bool BL::bidExist(const string &bid) const {
    // 用反向迭代器遍历
    vector<BaseBook>::const_reverse_iterator rit;
    for (rit = this->bl.rbegin(); rit != this->bl.rend(); ++rit) {
        if (rit->getBid() == bid) {
            return true;
        }
    }
    return false;
}

void BL::add(const BB &bb) {
    this->bl.push_back(bb);
}

void BL::del(const BB &bb) {
    this->del(bb.getBid());
}

void BL::del(const string &bid) {
    // 用迭代器遍历+erase
    vector<BaseBook>::iterator it;
    for (it = this->bl.begin(); it != this->bl.end(); ++it) {
        if (it->getBid() == bid) {
            this->bl.erase(it);
            return;
        }
    }
}

int BL::show() const {
    cout << "编号\t书籍id\t书籍名称\t作者\t出版社\t书籍总数\t借出数量" << endl;
    int count = 0;
    // 改用迭代器遍历输出
    vector<BaseBook>::const_iterator it;
    for (it = this->bl.begin(); it != this->bl.end(); ++it) {
        cout << ++count << ".\t";
        it->show();
    }
    return this->bl.size();
}

string BL::show(const int &num) const {
    this->bl[num].show();
    return this->bl[num].getBid();
}

void BL::show(const string &bid) const {
    // 用迭代器遍历查找
    vector<BaseBook>::const_iterator it;
    for (it = this->bl.begin(); it != this->bl.end(); ++it) {
        if (it->getBid() == bid) {
            it->show();
            return;
        }
    }
}

void BL::schBook(const string &s) const {
    cout << "编号\t书籍id\t书籍名称\t作者\t出版社\t书籍总数\t借出数量" << endl;
    int cnt = 0;
    // 用迭代器遍历匹配
    vector<BaseBook>::const_iterator it;
    for (it = this->bl.begin(); it != this->bl.end(); ++it) {
        if (it->match(s)) {
            cout << ++cnt << ".\t";
            it->show();
        }
    }
}

istream &operator>>(istream &in, BL &booklist) {
    int bookCount;
    in >> bookCount;
    // 改用while循环替换for循环，逻辑不变
    int i = 0;
    while (i < bookCount) {
        BaseBook bb;
        in >> bb;
        booklist.add(bb);
        i++;
    }
    return in;
}

ostream &operator<<(ostream &out, const BL &booklist) {
    out << booklist.size() << endl;
    // 用迭代器遍历输出
    vector<BaseBook>::const_iterator it;
    for (it = booklist.bl.begin(); it != booklist.bl.end(); ++it) {
        out << *it;
    }
    return out;
}