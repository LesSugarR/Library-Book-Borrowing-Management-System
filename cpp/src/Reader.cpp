//
// Created by LesSugarR on 25-12-10.
//
#include "../include/Reader.h"
#include  "../include/History.h"
#include <bits/stdc++.h>
#define BR BaseReader
#define RL ReaderList
#define BH BaseHistory
using namespace std;

bool BR::match(const string &s) const {
    return rid==s||rname==s;
}
void BR::show() const {
    cout<< rid << "  " << rname << "  " << borrowNum << endl;
}
// 【新增】基类的 toJson 实现
string BR::toJson() const {
    stringstream ss;
    ss << "{"
       << "\"rid\":\"" << rid << "\","
       << "\"rname\":\"" << rname << "\","
       << "\"borrowNum\":" << borrowNum
       << "}";
    return ss.str();
}
istream &operator >> (istream &in,BR &br){
    in>> br.rid >> br.rname >> br.borrowNum;
    return in;
}
ostream &operator << (ostream &out,const BR &br){
    out<< br.rid << "\n" << br.rname << "\n" << br.borrowNum << endl;
    return out;
}


//Reader:
void Reader::reqBorrow(const BH &bh){
    borrowReq.add(bh);
}
void Reader::agrBorrow(const BH &bh){
    borrowReq.del(bh);
    borrowHis.add(bh);
    borrowNum++;
}
void Reader::canBorrow(const BH &bh){
    borrowReq.del(bh);
    returnHis.add(bh);
}
void Reader::refBorrow(const BH &bh){
    borrowReq.del(bh);
    returnHis.add(bh);
}
void Reader::reqReturn(const BH &bh){
    borrowHis.del(bh);
    returnReq.add(bh);
}
void Reader::agrReturn(const BH &bh){
    returnReq.del(bh);
    returnHis.add(bh);
    borrowNum--;
}
void Reader::canReturn(const BH &bh){
    returnReq.del(bh);
    borrowHis.add(bh);// 撤销还书，书回到借阅列表
}
void Reader::refReturn(const BH &bh){
    returnReq.del(bh);
    borrowHis.add(bh);  // 拒绝还书，书回到借阅列表
}
int Reader::showBorrowReq() const {
    return borrowReq.show();
}
int Reader::showBorrowHis() const {
    return borrowHis.show();
}
int Reader::showReturnReq() const {
    return returnReq.show();
}
int Reader::showReturnHis() const {
    return returnHis.show();
}
string Reader::showBorrowReq(const int &num) const {
    return borrowReq.show(num);
}
string Reader::showBorrowHis(const int &num) const {
    return borrowHis.show(num);
}
string Reader::showReturnReq(const int &num) const {
    return returnReq.show(num);
}
string Reader::showReturnHis(const int &num) const {
    return returnHis.show(num);
}
// 【新增】Reader 的 toJson 实现
string Reader::toJson() const {
    stringstream ss;
    ss << "{"
       << "\"rid\":\"" << rid << "\","
       << "\"rname\":\"" << rname << "\","
       << "\"borrowNum\":" << borrowNum << ","
       // 注意：这里假设 HistoryList 有 showAll() 返回总数，如果没有请自行调整
       << "\"reqCount\":" << (borrowReq.showAll() + returnReq.showAll())
       << "}";
    return ss.str();
}
istream &operator >> (istream& in,Reader &r){
    in>> r.rid >> r.rname >> r.borrowNum >> r.rpasswd >> r.borrowReq >> r.borrowHis >> r.returnReq >> r.returnHis;
    return in;
}
ostream &operator << (ostream &out,const Reader &r){
    out<< r.rid << "\n" << r.rname << "\n" << r.borrowNum << "\n" << r.rpasswd << endl
       << r.borrowReq << r.borrowHis << r.returnReq << r.returnHis << endl;
    return out;
}

//ReaderList:

void ReaderList::clear() {
    while (head != nullptr) {
        BaseReaderNode* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    count = 0;
}

void ReaderList::add(const Reader &r) {
    BaseReaderNode *newNode = new BaseReaderNode(r);
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    ++count;
}
void ReaderList::del(const std::string &rid) {
    BaseReaderNode* curr = head;
    BaseReaderNode* prev = nullptr;

    while (curr != nullptr) {
        if (curr->data.getRid() == rid) {
            if (prev == nullptr) { // 删除头节点
                head = curr->next;
                if (tail == curr) tail = nullptr;
            } else {
                prev->next = curr->next;
                if (tail == curr) tail = prev;
            }
            delete curr;
            --count;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void ReaderList::del(const BaseReader &br) {
    del(br.getRid());
}
bool ReaderList::ridExist(const std::string &rid) const {
    BaseReaderNode* curr = head;
    while (curr != nullptr) {
        if (curr->data.getRid() == rid) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}
int ReaderList::show() const {
    std::cout << "编号  读者id  读者姓名  借阅数量" << std::endl;
    BaseReaderNode* curr = head;
    int index = 1;
    while (curr != nullptr) {
        std::cout << index << ".  ";
        curr->data.show();
        curr = curr->next;
        ++index;
    }
    return count;
}

std::string ReaderList::show(const int &num) const {
    if (num < 0 || num >= count) {
        std::cout << "序号超出范围！" << std::endl;
        return "";
    }
    BaseReaderNode* curr = head;
    for (int i = 0; i < num; ++i) {
        curr = curr->next;
    }
    curr->data.show();
    return curr->data.getRid();
}

void ReaderList::show(const std::string &rid) const {
    BaseReaderNode* curr = head;
    while (curr != nullptr) {
        if (curr->data.getRid() == rid) {
            curr->data.show();
            return;
        }
        curr = curr->next;
    }
}

void ReaderList::schReader(const std::string &s) const {
    std::cout << "编号  读者id  读者姓名  借阅数量" << std::endl;
    BaseReaderNode* curr = head;
    int cnt = 0;
    while (curr != nullptr) {
        if (curr->data.match(s)) {
            std::cout << ++cnt << ".  ";
            curr->data.show();
        }
        curr = curr->next;
    }
}
// 在 ReaderList 类中添加
string ReaderList::getRname(const string &rid) const {
    BaseReaderNode* curr = head;
    while (curr != nullptr) {
        if (curr->data.getRid() == rid) {
            return curr->data.getRname();
        }
        curr = curr->next;
    }
    return "";  // 返回空字符串表示未找到
}
// 【新增】JSON 字符串生成
string ReaderList::toJsonString() const {
    string json = "[";
    BaseReaderNode* curr = head;
    while (curr != nullptr) {
        json += curr->data.toJson();
        if (curr->next != nullptr) {
            json += ",";
        }
        curr = curr->next;
    }
    json += "]";
    return json;
}
// 【关键修改】返回类型改为 Reader&
Reader &ReaderList::getByRid(const string &rid) {
    BaseReaderNode* node = findNode(rid);
    if (node == nullptr) {
        throw runtime_error("该节点不存在: " + rid);
    }
    return node->data;
}
// 【关键修改】返回类型改为 Reader&
Reader &ReaderList::getByIndex(const int &index) {
    if (index < 0 || index >= count) {
        throw out_of_range("不符合范围: " + to_string(index));
    }
    BaseReaderNode* current = head;
    for (int i = 0; i < index && current != nullptr; i++) {
        current = current->next;
    }
    if (current == nullptr) {
        throw runtime_error("该读者无法找到: " + to_string(index));
    }
    return current->data;
}
// 辅助函数 findNode 实现
ReaderList::BaseReaderNode *ReaderList::findNode(const string &rid) {
    BaseReaderNode* current = head;
    while (current != nullptr) {
        if (current->data.getRid() == rid) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}
// 流操作符重载
std::istream &operator>>(std::istream &in, ReaderList &rl) {
    int num;
    if (!(in >> num)) return in; // 防止读取失败
    while (num--) {
        Reader r; // 修改为 Reader
        in >> r;
        rl.add(r);
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const ReaderList &rl) {
    out << rl.size() << std::endl;
    ReaderList::BaseReaderNode* curr = rl.head;
    while (curr != nullptr) {
        out << curr->data << std::endl;
        curr = curr->next;
    }
    return out;
}

