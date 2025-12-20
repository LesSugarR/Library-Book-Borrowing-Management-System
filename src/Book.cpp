#include "../include/Book.h"


// BaseBook 成员函数实现
bool BaseBook::match(const string &s) const {
    return bid==s||bname==s||author==s||press==s;
}
void BaseBook::show() const {
    cout << bid << "  " << bname << "  " << author << "  " << press << "  " << totalNum << "  " << borrowNum;
}
istream &operator >> (istream &in, BaseBook &bb){
    in >> bb.bid >> bb.bname >> bb.author >> bb.press >> bb.totalNum >> bb.borrowNum;
    return in;
}
ostream &operator << (ostream &out, const BaseBook &bb){
    out << bb.bid << "\n" << bb.bname << "\n" << bb.author << "\n" << bb.press << "\n" << bb.totalNum << "\n" << bb.borrowNum << endl;
    return out;
}

// Book 成员函数实现
void Book::addNum(const int &num){
    totalNum+=num;
}
void Book::reqBorrow(const BH &bh){
    borrowReq.add(bh);
}
void Book::agrBorrow(const BH &bh){
    borrowReq.del(bh);
    borrowHis.add(bh);
    borrowNum++;
}
void Book::canBorrow(const BH &bh){
    borrowReq.del(bh);
    returnHis.add(bh);
}
void Book::refBorrow(const BH &bh){
    borrowReq.del(bh);
    returnHis.add(bh);
}
void Book::reqReturn(const BH &bh){
    borrowHis.del(bh);
    returnReq.add(bh);
}
void Book::agrReturn(const BH &bh){
    returnReq.del(bh);
    returnHis.add(bh);
    borrowNum--;
}
void Book::canReturn(const BH &bh){
    returnReq.del(bh);
    borrowHis.add(bh);
}
void Book::refReturn(const BH &bh){
    returnReq.del(bh);
    borrowHis.add(bh);
}
int Book::showBorrowReq() const {
    return borrowReq.showAll();
}
int Book::showBorrowHis() const {
    return borrowHis.showAll();
}
int Book::showReturnReq() const {
    return returnReq.showAll();
}
int Book::showReturnHis() const {
    return returnHis.showAll();
}
ostream &operator << (ostream &out, const Book &b){
    out << b.bid << "\n" << b.bname << "\n" << b.author << "\n" << b.press << "\n" << b.totalNum << "\n" << b.borrowNum << endl;
    out << b.borrowReq << b.borrowHis << b.returnReq << b.returnHis;
    return out;
}

// BookNode 构造函数
BookList::BookNode::BookNode(const BaseBook& book) : data(book), next(nullptr) {}

// BookList 私有辅助函数
BookList::BookNode* BookList::findNode(const string& bid) const {
    BookNode* current = head;
    while (current != nullptr) {
        if (current->data.getBid() == bid) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

BookList::BookNode* BookList::findNode(int index) const {
    if (index < 0 || index >= count) {
        return nullptr;
    }
    BookNode* current = head;
    for (int i = 0; i < index && current != nullptr; i++) {
        current = current->next;
    }
    return current;
}

void BookList::clear() {
    BookNode* current = head;
    while (current != nullptr) {
        BookNode* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;
    count = 0;
}

// BookList 公有成员函数
BookList::BookList() : head(nullptr), tail(nullptr), count(0) {}

BookList::~BookList() {
    clear();
}

BookList::BookList(const BookList& other) : head(nullptr), tail(nullptr), count(0) {
    BookNode* current = other.head;
    while (current != nullptr) {
        add(current->data);
        current = current->next;
    }
}

BookList& BookList::operator=(const BookList& other) {
    if (this != &other) {
        clear();
        BookNode* current = other.head;
        while (current != nullptr) {
            add(current->data);
            current = current->next;
        }
    }
    return *this;
}

BaseBook& BookList::getByBid(const string &bid) {
    BookNode* node = findNode(bid);
    if (node == nullptr) {
        throw runtime_error("Book not found with bid: " + bid);
    }
    return node->data;
}

BaseBook& BookList::getByIndex(int index) {
    BookNode* node = findNode(index);
    if (node == nullptr) {
        throw out_of_range("Index out of range: " + to_string(index));
    }
    return node->data;
}

bool BookList::bidExist(const string &bid) const {
    return findNode(bid) != nullptr;
}

void BookList::add(const BaseBook &bb){
    BookNode* newNode = new BookNode(bb);

    if (head == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    count++;
}

void BookList::del(const BaseBook &bb){
    del(bb.getBid());
}

void BookList::del(const string &bid){
    if (head == nullptr) return;

    // 删除头节点
    if (head->data.getBid() == bid) {
        BookNode* temp = head;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        delete temp;
        count--;
        return;
    }

    // 删除中间或尾部节点
    BookNode* prev = head;
    BookNode* current = head->next;

    while (current != nullptr) {
        if (current->data.getBid() == bid) {
            prev->next = current->next;
            if (current == tail) {
                tail = prev;
            }
            delete current;
            count--;
            return;
        }
        prev = current;
        current = current->next;
    }
}

int BookList::show() const {
    cout << "编号  书籍id  书籍名称  作者  出版社  书籍总数  借出数量" << endl;
    int index = 0;
    BookNode* current = head;

    while (current != nullptr) {
        cout << ++index << ".  ";
        current->data.show();
        cout << endl;
        current = current->next;
    }

    return count;
}

string BookList::show(const int &num) const {
    BookNode* node = findNode(num);
    if (node != nullptr) {
        node->data.show();
        cout << endl;
        return node->data.getBid();
    }
    return "";
}

void BookList::show(const string &bid) const {
    BookNode* node = findNode(bid);
    if (node != nullptr) {
        node->data.show();
        cout << endl;
    }
}
string BookList::getBname(const string &bid){
    BookNode* node = findNode(bid);
    if (node == nullptr) {
        return "";
    }
    return node->data.getBname();
}
void BookList::schBook(const string &s) const {
    cout << "编号  书籍id  书籍名称  作者  出版社  书籍总数  借出数量" << endl;
    int cnt = 0;
    BookNode* current = head;

    while (current != nullptr) {
        if (current->data.match(s)) {
            cout << ++cnt << ".  ";
            current->data.show();
            cout << endl;
        }
        current = current->next;
    }

    if (cnt == 0) {
        cout << "未找到匹配的书籍" << endl;
    }
}

// BookList 友元函数
istream &operator >> (istream &in, BookList &bl){
    int num;
    in >> num;

    bl.clear(); // 清空原有数据
    for (int i = 0; i < num; i++) {
        BaseBook bb;
        in >> bb;
        bl.add(bb);
    }

    return in;
}

ostream &operator << (ostream &out, const BookList &bl){
    out << bl.count << endl;

    BookList::BookNode* current = bl.head;
    while (current != nullptr) {
        out << current->data;
        current = current->next;
    }

    return out;
}