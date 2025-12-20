//
// Created by LesSugarR on 25-12-10.
//
//

#include "../include/Book.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace LibraryBookSystem;

//BaseBook 成员函数实现

// 构造函数
BaseBook::BaseBook(std::string _bid, std::string _bname,
                   std::string _author, std::string _press,
                   int _totalNum, int _borrowNum,
                   std::string _inTime)
    : bid(_bid), bname(_bname), author(_author), press(_press),
      totalNum(_totalNum), borrowNum(_borrowNum), inTime(_inTime.empty() ? getCurrentTime() : _inTime) {

    // 基本数据验证
    if (totalNum < 0) {
        throw invalid_argument("总数量不能为负数");
    }
    if (borrowNum < 0) {
        throw invalid_argument("借出数量不能为负数");
    }
    if (borrowNum > totalNum) {
        throw invalid_argument("借出数量不能超过总数量");
    }
}

// 获取当前时间
std::string BaseBook::getCurrentTime() {
    time_t now = time(0);
    tm* localtm = localtime(&now);
    ostringstream oss;
    oss << put_time(localtm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

// Getter函数
std::string BaseBook::getBid() const { return bid; }
std::string BaseBook::getBname() const { return bname; }
std::string BaseBook::getAuthor() const { return author; }
std::string BaseBook::getPress() const { return press; }
int BaseBook::getTotalNum() const { return totalNum; }
int BaseBook::getBorrowNum() const { return borrowNum; }
std::string BaseBook::getInTime() const { return inTime; }

// Setter函数
void BaseBook::setBid(const std::string& id) {
    if (id.empty()) {
        throw invalid_argument("图书ID不能为空");
    }
    bid = id;
}

void BaseBook::setBname(const std::string& name) {
    if (name.empty()) {
        throw invalid_argument("书名不能为空");
    }
    bname = name;
}

void BaseBook::setAuthor(const std::string& auth) {
    if (auth.empty()) {
        throw invalid_argument("作者不能为空");
    }
    author = auth;
}

void BaseBook::setPress(const std::string& p) {
    if (p.empty()) {
        throw invalid_argument("出版社不能为空");
    }
    press = p;
}

void BaseBook::setTotalNum(int num) {
    if (num < 0) {
        throw invalid_argument("总数量不能为负数");
    }
    if (num < borrowNum) {
        throw invalid_argument("总数量不能少于已借出数量");
    }
    totalNum = num;
}

void BaseBook::setBorrowNum(int num) {
    if (num < 0) {
        throw invalid_argument("借出数量不能为负数");
    }
    if (num > totalNum) {
        throw invalid_argument("借出数量不能超过总数量");
    }
    borrowNum = num;
}

void BaseBook::setInTime(const std::string& time) {
    if (time.empty()) {
        throw invalid_argument("入库时间不能为空");
    }
    inTime = time;
}

// 判断函数
bool BaseBook::canBorrow() const {
    return (totalNum - borrowNum) > 0;
}

bool BaseBook::canDelete() const {
    return borrowNum == 0;
}

// 匹配函数
bool BaseBook::match(const std::string& keyword) const {
    if (keyword.empty()) {
        return false;
    }

    // 简单的字符串查找
    if (bname.find(keyword) != string::npos) return true;
    if (bid.find(keyword) != string::npos) return true;
    if (author.find(keyword) != string::npos) return true;
    if (press.find(keyword) != string::npos) return true;

    return false;
}

// 显示图书信息
void BaseBook::show() const {
    cout << bid << "\t" << bname << "\t" << author
         << "\t" << press << "\t" << totalNum
         << "\t" << borrowNum << "\t" << inTime << endl;
}

// BookNode 成员函数实现

BookNode::BookNode(const BaseBook& book) : data(book), next(nullptr) {}
BookNode::BookNode() : data(), next(nullptr) {}

//  BookList 成员函数实现

// 构造函数
BookList::BookList() : head(nullptr), tail(nullptr), size(0) {}

// 析构函数
BookList::~BookList() {
    clear();
}

// 清空链表
void BookList::clear() {
    BookNode* current = head;
    while (current != nullptr) {
        BookNode* temp = current;
        current = current->next;
        delete temp;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

int BookList::getSize() const { return size; }

bool BookList::bidExists(const std::string& bid) const {
    BookNode* current = head;
    while (current != nullptr) {
        if (current->data.getBid() == bid) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void BookList::add(const BaseBook& book) {
    if (bidExists(book.getBid())) {
        throw runtime_error("书籍ID已存在，无法重复添加！");
    }

    BookNode* newNode = new BookNode(book);

    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }

    size++;
}

void BookList::remove(const std::string& bid) {
    BookNode* prev = nullptr;
    BookNode* current = head;

    while (current != nullptr && current->data.getBid() != bid) {
        prev = current;
        current = current->next;
    }

    if (current == nullptr) {
        throw runtime_error("书籍ID不存在，无法删除！");
    }

    if (!current->data.canDelete()) {
        throw runtime_error("书籍仍有借出，无法删除！");
    }

    if (prev == nullptr) {
        head = current->next;
    } else {
        prev->next = current->next;
    }

    if (current == tail) {
        tail = prev;
    }

    delete current;
    size--;
}

void BookList::remove(const BaseBook& book) {
    remove(book.getBid());
}

void BookList::update(const BaseBook& book) {
    BookNode* node = getNodeByBid(book.getBid());
    if (node == nullptr) {
        throw runtime_error("书籍ID不存在，无法更新！");
    }

    if (book.getBorrowNum() > book.getTotalNum()) {
        throw invalid_argument("借出数量不能超过总数量");
    }

    node->data = book;
}

BaseBook& BookList::getByBid(const std::string& bid) {
    BookNode* node = getNodeByBid(bid);
    if (node == nullptr) {
        throw runtime_error("书籍ID不存在！");
    }
    return node->data;
}

const BaseBook& BookList::getByBid(const std::string& bid) const {
    const BookNode* node = getNodeByBid(bid);
    if (node == nullptr) {
        throw runtime_error("书籍ID不存在！");
    }
    return node->data;
}

BaseBook& BookList::getByIndex(int index) {
    if (index < 0 || index >= size) {
        throw out_of_range("索引越界！");
    }

    BookNode* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    return current->data;
}

const BaseBook& BookList::getByIndex(int index) const {
    if (index < 0 || index >= size) {
        throw out_of_range("索引越界！");
    }

    BookNode* current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }

    return current->data;
}

void BookList::showAll() const {
    if (size == 0) {
        cout << "当前没有图书" << endl;
        return;
    }

    cout << "编号\t书籍id\t书籍名称\t作者\t出版社\t书籍总数\t借出数量\t入库时间" << endl;
    int count = 0;
    BookNode* cur = head;
    while (cur != nullptr) {
        cout << ++count << ".\t";
        cur->data.show();
        cur = cur->next;
    }
}

void BookList::search(const std::string& keyword) const {
    if (size == 0) {
        cout << "当前没有图书" << endl;
        return;
    }

    cout << "编号\t书籍id\t书籍名称\t作者\t出版社\t书籍总数\t借出数量\t入库时间" << endl;
    int cnt = 0;
    BookNode* cur = head;
    while (cur != nullptr) {
        if (cur->data.match(keyword)) {
            cout << ++cnt << ".\t";
            cur->data.show();
        }
        cur = cur->next;
    }

    if (cnt == 0) {
        cout << "未找到匹配书籍！" << endl;
    }
}

void BookList::sortByInTime(bool ascending) {
    if (size <= 1) return;

    // 简单插入排序
    BookNode* sortedHead = nullptr;
    BookNode* current = head;

    while (current != nullptr) {
        BookNode* nextNode = current->next;

        BookNode* prev = nullptr;
        BookNode* search = sortedHead;

        while (search != nullptr) {
            bool shouldInsertBefore = ascending ?
                (current->data.getInTime() < search->data.getInTime()) :
                (current->data.getInTime() > search->data.getInTime());

            if (shouldInsertBefore) break;

            prev = search;
            search = search->next;
        }

        if (prev == nullptr) {
            current->next = sortedHead;
            sortedHead = current;
        } else {
            current->next = prev->next;
            prev->next = current;
        }

        current = nextNode;
    }

    head = sortedHead;

    tail = head;
    if (tail != nullptr) {
        while (tail->next != nullptr) {
            tail = tail->next;
        }
    }
}

BaseBook& BookList::operator[](const std::string& bid) {
    return getByBid(bid);
}

BaseBook& BookList::operator[](int index) {
    return getByIndex(index);
}

const BaseBook& BookList::operator[](const std::string& bid) const {
    return getByBid(bid);
}

const BaseBook& BookList::operator[](int index) const {
    return getByIndex(index);
}

// 简单的文件格式输入输出
std::istream& operator>>(std::istream& in, BaseBook& book) {
    string tempBid, tempName, tempAuthor, tempPress, tempInTime;
    int tempTotal, tempBorrow;

    in >> tempBid >> tempName >> tempAuthor >> tempPress >> tempTotal >> tempBorrow >> tempInTime;

    if (tempBid.empty()) throw runtime_error("书籍ID不能为空！");
    if (tempTotal < 0 || tempBorrow < 0) throw runtime_error("书籍数量不能为负数！");
    if (tempBorrow > tempTotal) throw runtime_error("借出数量不能超过总数量！");

    book = BaseBook(tempBid, tempName, tempAuthor, tempPress, tempTotal, tempBorrow, tempInTime);

    return in;
}

std::ostream& operator<<(std::ostream& out, const BaseBook& book) {
    out << book.getBid() << endl
        << book.getBname() << endl
        << book.getAuthor() << endl
        << book.getPress() << endl
        << book.getTotalNum() << endl
        << book.getBorrowNum() << endl
        << book.getInTime() << endl;
    return out;
}

// 私有辅助函数
BookNode* BookList::getNodeAt(int index) const {
    if (index < 0 || index >= size) {
        return nullptr;
    }

    BookNode* current = head;
    for (int i = 0; i < index && current != nullptr; i++) {
        current = current->next;
    }

    return current;
}

BookNode* BookList::findPrevByBid(const std::string& bid) const {
    BookNode* prev = nullptr;
    BookNode* current = head;

    while (current != nullptr && current->data.getBid() != bid) {
        prev = current;
        current = current->next;
    }

    return (current != nullptr) ? prev : nullptr;
}

BookNode* BookList::getNodeByBid(const std::string& bid) const {
    BookNode* current = head;
    while (current != nullptr) {
        if (current->data.getBid() == bid) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}
