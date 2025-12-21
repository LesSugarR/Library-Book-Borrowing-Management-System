//
// Created by LesSugarR on 25-12-10.
//
#include "../include/History.h"
#include "../include/Functions.h"
#include <bits/stdc++.h>
#define RC Record
#define RL RecordList
#define BH BaseHistory
#define HL HistoryList
using namespace std;
static string getTime() {
    static int counter = 0;
    return "T" + to_string(++counter); // 简单时间戳：T1, T2, T3...
}
//BaseHistory
bool BH::match(const string &s) const {
    return hid==s||bid==s||rid==s||bname==s||rname==s||note==s;
}
istream  &operator >>(istream &in,BaseHistory &bh){
    in>> bh.hid >> bh.bid >> bh.rid >> bh.bname >> bh.rname >> bh.note
      >> bh.brtime >> bh.btime >> bh.rrtime >> bh.rtime;
    return in;
}
ostream &operator << (ostream &out,const BH &bh){
    out<< bh.hid << "\n" << bh.bid << "\n" << bh.rid << "\n" << bh.bname << "\n" << bh.rname << "\n"
       << bh.note << "\n" << bh.brtime << "\n" << bh.btime << "\n" << bh.rrtime << "\n"
       << bh.rtime << endl;
    return out;
}
void BH::display() const {
    cout<< hid << "  " << rid << "  " << rname << "  " << bid << "  " << bname << "  "
        << btime << "  " << rtime << "  " << note << endl;
}
//History::Record
istream &operator >> (istream &in,History::Record &hr){
    in>> hr.time >> hr.note;
    return in;
}
ostream &operator << (ostream &out,const History::Record &hr){
    out<< hr.time << "\n" << hr.note << endl;
    return out;
}
void History::RC::display() const {
    cout<<time<<" "<<note<<" "<<endl;
}
//History:
void History::reqBorrow() {
    string t=getTime();
    recordLinkedList.append(Record(t,"申请借书"));
    note="借书申请待审核";
    brtime=t;
}
void History::agrBorrow(){
    string t=getTime();
    recordLinkedList.append(Record(t,"同意借书"));
    note="借出中";
    btime=t;
}
void History::canBorrow(){
    string t=getTime();
    recordLinkedList.append(Record(t,"取消借书"));
    note="借书申请已撤销";
}
void History::refBorrow(){
    string t=getTime();
    recordLinkedList.append(Record(t,"拒绝借书"));
    note="借书申请被拒绝";
}
void History::reqReturn(){
    string t=getTime();
    recordLinkedList.append(Record(t,"申请还书"));
    note="还书申请待审核";
    rrtime=t;
}
void History::agrReturn(){
    string t=getTime();
    recordLinkedList.append(Record(t,"同意还书"));
    note="已归还";
    rtime=t;
}
void History::canReturn(){
    string t=getTime();
    recordLinkedList.append(Record(t,"取消还书"));
    note="还书申请已撤销";
}
void History::refReturn(){
    string t=getTime();
    recordLinkedList.append(Record(t,"拒绝还书"));
    note="还书申请被拒绝";
}
istream &operator >> (istream &in,History &h){
    in>> h.hid >> h.bid >> h.rid >> h.bname >> h.rname >> h.note
      >> h.brtime >> h.btime >> h.rrtime >> h.rtime;
    int num; in>> num;
    while(num--){ History::Record hr; in>> hr; h.recordLinkedList.append(hr); }
    return in;
}
ostream &operator << (ostream &out,const History &h){
    out<< h.hid << "\n" << h.bid << "\n" << h.rid << "\n" << h.bname << "\n" << h.rname << "\n"
       << h.note << "\n" << h.brtime << "\n" << h.btime << "\n" << h.rrtime << "\n"
       << h.rtime << endl;
    out << h.recordLinkedList.getCount() << "\n";
    return out;
}
//HistoryList
void HistoryList::clear() {
    while (head != nullptr) {
        BaseHistoryNode* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    count = 0;
}

void HistoryList::add(const BaseHistory& bh) {
    BaseHistoryNode* newNode = new BaseHistoryNode(bh);
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    ++count;
}
void HistoryList::del(const BaseHistory &bh) {
    del(bh.getHid());
}

void HistoryList::del(const std::string &hid) {
    BaseHistoryNode* curr = head;
    BaseHistoryNode* prev = nullptr;

    while (curr != nullptr) {
        if (curr->data.getHid() == hid) {
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

int HistoryList::showAll() const {
    if (head == nullptr) {
        std::cout << "暂无历史记录。" << std::endl;
        return 0;
    }
    std::cout << "编号  历史记录ID  读者ID  读者姓名  书籍ID  书籍名称  借书时间  还书时间  借阅状态" << std::endl;
    int idx = 1;
    BaseHistoryNode* curr = head;
    while (curr != nullptr) {
        std::cout << idx++ << ".  ";
        curr->data.display();
        curr = curr->next;
    }
}
void HistoryList::show(const string &hid) const{
    if (head == nullptr) {
        std::cout << "暂无历史记录。" << std::endl;
        return;
    }

    BaseHistoryNode *curr=head;
    while (curr != nullptr) {
        if (curr->data.getHid()==hid){
            std::cout<<"找到该历史记录"<<std::endl;
            curr->data.display();
            return;
        }
    }
}
string  HistoryList::show(const int &num) const{
    BaseHistoryNode *cur=head;
    for (int i = 0; i < num; ++i) {
        cur=cur->next;
    }
    cur->data.display();
    return cur->data.getHid();
}
BaseHistory* HistoryList::findByHid(const std::string& hid) {
    BaseHistoryNode* curr = head;
    while (curr != nullptr) {
        if (curr->data.getHid() == hid) {
            return &(curr->data);
        }
        curr = curr->next;
    }
    return nullptr;
}

void HistoryList::schHistory(const std::string &s) const {
    if (head == nullptr) {
        std::cout << "暂无历史记录。" << std::endl;
        return;
    }

    std::cout << "编号  历史记录ID  读者ID  读者姓名  书籍ID  书籍名称  借书时间  还书时间  借阅状态" << std::endl;
    BaseHistoryNode* curr = head;
    int foundCount = 0;

    while (curr != nullptr) {
        if (curr->data.match(s)) {
            std::cout << ++foundCount << ".  ";
            curr->data.display();
        }
        curr = curr->next;
    }

    if (foundCount == 0) {
        std::cout << "未找到相关历史记录。" << std::endl;
    }
}
std::istream& operator>>(std::istream& in, HistoryList& hl) {
    int n;
    in >> n;
    for (int i = 0; i < n; ++i) {
        BaseHistory bh;
        in >> bh;
        hl.add(bh);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const HistoryList& hl) {
    out << hl.count << "\n";
    HL::BaseHistoryNode* curr = hl.head;
    while (curr != nullptr) {
        out << curr->data << "\n";
        curr = curr->next;
    }
    return out;
}

BaseHistory HistoryList::getByHid(const string &hid) {
    BaseHistoryNode* node = findNode(hid);  // 需要实现 findNode 方法
    if (node == nullptr) {
        throw runtime_error("历史记录中不存在该id: " + hid);
    }
    return node->data;
}

HistoryList::BaseHistoryNode *HistoryList::findNode(const string &hid) {
    BaseHistoryNode* current = head;
    while (current != nullptr) {
        if (current->data.getHid() == hid) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}
