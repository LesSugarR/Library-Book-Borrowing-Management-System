#include "../include/History.h"
#include "../include/Functions.h"
#include <bits/stdc++.h>
#define RC Record
#define RL RecordList
#define BH BaseHistory
#define HL HistoryList
using namespace std;
//BaseHistory
bool BH::match(const string &s) const {
    return hid==s||bid==s||rid==s||bname==s||rname==s||note==s;
}
void BH::display() const {
    cout<< hid << "  " << rid << "  " << rname << "  " << bid << "  " << bname << "  "
        << btime << "  " << rtime << "  " << note << endl;
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

//History::Record
void History::Record::display() const {
    cout << time << " " << note << endl;
}

istream &operator>>(istream &in, History::Record &hr) {
    in >> hr.time >> hr.note;
    return in;
}

ostream &operator<<(ostream &out, const History::Record &hr) {
    out << hr.time << "\n" << hr.note << endl;
    return out;
}


//深拷贝实现
History::RecordLinkedList::RecordLinkedList(const RecordLinkedList& other) : head(nullptr), tail(nullptr), count(0) {
    RecordNode* curr = other.head;
    while(curr) {
        append(curr->data);
        curr = curr->next;
    }
}
History::RecordLinkedList& History::RecordLinkedList::operator=(const RecordLinkedList& other) {
    if (this != &other) {
        clear();
        RecordNode* curr = other.head;
        while(curr) {
            append(curr->data);
            curr = curr->next;
        }
    }
    return *this;
}
void History::RecordLinkedList::clear() {
    while (head != nullptr) {
        RecordNode* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    count = 0;
}
void History::RecordLinkedList::append(const Record &record){
    RecordNode* newNode = new RecordNode(record);
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    ++count;
}
string History::RecordLinkedList::toJsonString() const {
    string json = "[";
    RecordNode* curr = head;
    while(curr) {
        // 【修改】使用函数来获取数据
        json += "{\"time\":\"" + curr->data.getTime() + "\", \"note\":\"" + curr->data.getNote() + "\"}";
        if(curr->next) json += ",";
        curr = curr->next;
    }
    json += "]";
    return json;
}
// 输出链表内容（用于文件保存）
ostream &operator<<(ostream &out, const History::RecordLinkedList &list)
{
    History::RecordNode* curr = list.head;
    while (curr != nullptr) {
        out << curr->data; // 利用 Record 的 operator<<
        curr = curr->next;
    }
    return out;
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
      >> h.brtime >> h.btime >> h.rrtime >> h.rtime >> h.status;
    int num; in>> num;
    h.recordLinkedList.clear();
    while(num--){ History::Record hr; in>> hr; h.recordLinkedList.append(hr); }
    return in;
}
ostream &operator << (ostream &out,const History &h){
    out<< h.hid << "\n" << h.bid << "\n" << h.rid << "\n" << h.bname << "\n" << h.rname << "\n"
       << h.note << "\n" << h.brtime << "\n" << h.btime << "\n" << h.rrtime << "\n"
       << h.rtime << "\n" << h.status <<  endl;
    out << h.recordLinkedList.getCount() << "\n";
    out << h.recordLinkedList;
    return out;
}
string History::toJson() const {
    return "{\"hid\":\"" + hid + "\", \"bid\":\"" + bid + "\", \"bname\":\"" + bname +
           "\", \"rid\":\"" + rid + "\", \"rname\":\"" + rname + "\", \"status\":\"" + note +
           "\", \"records\":" + recordLinkedList.toJsonString() + "}";
}
//HistoryList
HistoryList::HistoryList(const HistoryList& other) : head(nullptr), tail(nullptr), count(0) {
    HistoryNode* curr = other.head;
    while(curr) {
        add(curr->data);
        curr = curr->next;
    }
}
HistoryList& HistoryList::operator=(const HistoryList& other) {
    if (this != &other) {
        clear();
        HistoryNode* curr = other.head;
        while(curr) {
            add(curr->data);
            curr = curr->next;
        }
    }
    return *this;
}
void HistoryList::clear() {
    while (head != nullptr) {
        HistoryNode* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    count = 0;
}

void HistoryList::add(const History& h) {
    HistoryNode* newNode = new HistoryNode(h);
    if (tail == nullptr) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    ++count;
}
//兼容性新增，允许添加 BaseHistory，自动转换为 History
void HistoryList::add(const BaseHistory& bh) {
    History h(bh); // 使用 History(BaseHistory) 构造函数
    add(h);
}
void HistoryList::del(const BaseHistory &bh) {
    del(bh.getHid());
}
void HistoryList::showForApi()
{
    cout << count << endl; // 第1行：总数
    HistoryNode* current = head;
    while (current != nullptr) {
        // 严格按照顺序输出 11 个字段
        cout << current->data.getHid() << endl;
        cout << current->data.getBid() << endl;
        cout << current->data.getRid() << endl;
        cout << current->data.getBname() << endl;
        cout << current->data.getRname() << endl;
        cout << current->data.getNote() << endl;
        cout << current->data.getBrtime() << endl;
        cout << current->data.getBtime() << endl;
        cout << current->data.getRrtime() << endl;
        cout << current->data.getRtime() << endl;
        cout << current->data.getStatus() << endl;

        current = current->next;
    }
}
void HistoryList::del(const string &hid) {
    if (!head) return;

    // 删除头节点
    if (head->data.getHid() == hid) {
        HistoryNode* temp = head;
        head = head->next;
        if (head == nullptr) tail = nullptr;
        delete temp;
        --count;
        return;
    }
    // 删除中间或尾部节点
    HistoryNode* curr = head->next;
    HistoryNode* prev = head;
    while (curr != nullptr) {
        if (curr->data.getHid() == hid) {
            prev->next = curr->next;
            if (curr == tail) tail = prev;
            delete curr;
            --count;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}
int HistoryList::show() const {
    if (head == nullptr) {
         cout << "暂无历史记录。" << endl;
        return 0;
    }
    cout << "编号  历史记录ID  读者ID  读者姓名  书籍ID  书籍名称  借书时间  还书时间  借阅状态" << endl;
    int idx = 0;
    HistoryNode* curr = head;
    while (curr != nullptr) {
        cout << ++idx << ".  ";
        curr->data.display();
        curr = curr->next;
    }
    return count; // 【修复】返回列表大小，保持兼容
}
void HistoryList::show(const string &hid) const {
    HistoryNode* curr = head;
    while (curr != nullptr) {
        if (curr->data.getHid() == hid) {
            curr->data.display();
            return;
        }
        curr = curr->next;
    }
}
string HistoryList::show(const int &num) const {
    HistoryNode* node = findNode(num);
    if (node) {
        node->data.display();
        return node->data.getHid();
    }
    return "";
}
void HistoryList::schHistory(const string &s) const {
    if (head == nullptr) {
        std::cout << "暂无历史记录。" << std::endl;
        return;
    }
    cout << "编号  历史记录ID  读者ID  读者姓名  书籍ID  书籍名称  借书时间  还书时间  借阅状态" << endl;
    HistoryNode* curr = head;
    int foundCount = 0;
    while (curr != nullptr) {
        if (curr->data.match(s)) {
            cout << ++foundCount << ".  ";
            curr->data.display();
        }
        curr = curr->next;
    }
    if (foundCount == 0) {
        cout << "未找到相关历史记录。" << endl;
    }
}
// 辅助函数：按索引查找节点
HistoryList::HistoryNode* HistoryList::findNode(int index) const {
    if (index < 0 || index >= count) return nullptr;
    HistoryNode* curr = head;
    for (int i = 0; i < index && curr != nullptr; ++i) {
        curr = curr->next;
    }
    return curr;
}

// 辅助函数：按ID查找节点
HistoryList::HistoryNode* HistoryList::findNode(const string &hid) const { // 修复参数类型
    HistoryNode* curr = head;
    while (curr != nullptr) {
        if (curr->data.getHid() == hid) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}
// 【兼容性新增】下标运算符重载 (String)
BaseHistory &HistoryList::operator [](const string& hid) {
    HistoryNode* node = findNode(hid);
    if (node == nullptr) {
        throw runtime_error("History ID not found: " + hid);
    }
    return node->data; // 返回 BaseHistory& (History 是 BaseHistory 子类)
}

// 【兼容性新增】下标运算符重载 (Int)
BaseHistory &HistoryList::operator [](const int& index) {
    HistoryNode* node = findNode(index);
    if (node == nullptr) {
        throw out_of_range("HistoryList index out of range");
    }
    return node->data;
}
History &HistoryList::getByHid(const string &hid) {
    HistoryNode* node = findNode(hid);

    if (node == nullptr) {
        // 千万不要 return History();
        // 像 ReaderList 那样抛出异常，这样最安全，也方便排错
        throw runtime_error("未找到该历史记录: " + hid);
    }

    return node->data; // 返回节点里的数据引用
}

BaseHistory &HistoryList::getByIndex(int index) {
    return (*this)[index]; // 复用 operator[]
}
BaseHistory* HistoryList::findByHid(const string& hid) {
    HistoryNode* node = findNode(hid); // 修正调用
    if (node) return &(node->data);
    return nullptr;
}
string HistoryList::toJsonArray() const {
    string json = "[";
    HistoryNode* curr = head;
    while(curr) {
        json += curr->data.toJson();
        if(curr->next) json += ",";
        curr = curr->next;
    }
    json += "]";
    return json;
}

istream& operator>>(istream& in, HistoryList& hl) {
    int n = 0;
    if (!(in >> n)) return in;
    hl.clear();
    string tmp;
    getline(in, tmp); //去掉n后面的换行符
    for(int i=0;i<n;i++){
        string hid,bid,rid,bname,rname,note,brtime,btime,rrtime,rtime,status;
        getline(in, hid);
        getline(in, bid);
        getline(in, rid);
        getline(in, bname);
        getline(in, rname);
        getline(in, note);
        getline(in, brtime);
        getline(in, btime);
        getline(in, rrtime);
        getline(in, rtime);
        getline(in, status);
        //如果读到的是占位符 "-"，则还原为空字符串
        if (note == "-") note = "";
        if (brtime == "-") brtime = "";
        if (btime == "-") btime = "";
        if (rrtime == "-") rrtime = "";
        if (rtime == "-") rtime = "";
        BH bh(hid,bid,rid,bname,rname,note,brtime,btime,rrtime,rtime);
        bh.setStatus(status);
        hl.add(bh);
    }
    return in;
}

ostream& operator<<(ostream& out, const HistoryList& hl)
{
    out << hl.size() << '\n';
    const HistoryList::HistoryNode* curr = hl.head;
    while (curr)
    {
        const BH& data = curr->data;
        out << (data.getHid().empty() ? "-" : data.getHid()) << endl;
        out << (data.getBid().empty() ? "-" : data.getBid()) << endl;
        out << (data.getRid().empty() ? "-" : data.getRid()) << endl;
        out << (data.getBname().empty() ? "-" : data.getBname()) << endl;
        out << (data.getRname().empty() ? "-" : data.getRname()) << endl;
        out << (data.getNote().empty() ? "-" : data.getNote()) << endl;
        out << (data.getBrtime().empty() ? "-" : data.getBrtime()) << endl;
        out << (data.getBtime().empty() ? "-" : data.getBtime()) << endl;
        out << (data.getRrtime().empty() ? "-" : data.getRrtime()) << endl;
        out << (data.getRtime().empty() ? "-" : data.getRtime()) << endl;
        out << (data.getStatus().empty() ? "-" : data.getStatus()) << endl;
        curr = curr->next;
    }
    return out;
}
