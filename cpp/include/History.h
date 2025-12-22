
#ifndef HISTORY_H
#define HISTORY_H
#include<bits/stdc++.h>
using namespace std;

class BaseHistory{
protected:
    /** @brief 历史记录的唯一id*/
    string hid;
    /** @brief 图书id*/
    string bid;
    /** @brief 读者id*/
    string rid;
    /** @brief 图书名称*/
    string bname;
    /** @brief 读者姓名*/
    string rname;
    /** @brief 管理员备注信息*/
    string note;
    /** @brief 借阅时间*/
    string brtime;
    /**@brief 管理员同意借出时间*/
    string btime;
    /** @brief 申请归还时间*/
    string rrtime;
    /** @brief 实际归还时间*/
    string rtime;
public:
    BaseHistory(string _hid,string _bid,string _rid,string _bname,string _rname,string _note="无",string _brtime="无",string _btime="无",string _rrtime="无",string _rtime="无"):
            hid(_hid),bid
            (_bid),rid(_rid),bname(_bname),rname(_rname),note(_note),brtime(_brtime),btime(_btime),rrtime(_rrtime),rtime(_rtime){}
    //参数兼容用
    BaseHistory(string _hid, string _bid, string _rid, string _bname, string _rname, string _date, int _state)
            : hid(_hid), bid(_bid), rid(_rid), bname(_bname), rname(_rname), brtime(_date), note(to_string(_state)) {}
    BaseHistory(){}
    virtual ~BaseHistory(){}
    string getHid() const { return hid; }
    string getBid()const { return bid; }
    string getRid() const { return rid; }
    string getBname() const { return bname; }
    string getRname() const { return rname; }
    string getNote() const { return note; }
    string getBrtime() const { return brtime; }
    string getBtime() const { return btime; }
    string getRrtime() const { return rrtime; }
    string getRtime() const { return rtime; }
    virtual void display() const;//【增加了virtual】
    void show()const
    {
        display();
    }
    bool match(const string &s) const;
    friend istream &operator >> (istream &in,BaseHistory &bh);
    friend ostream &operator << (ostream &out,const BaseHistory &bh);
};

class History:public BaseHistory{
public:
    class Record{
        string time,note;


    public:
        Record(string _time,string _note):time(_time),note(_note){}
        Record(){}
        string getTime() const { return time; }
        string getNote() const { return note; }
        friend istream &operator >> (istream &in,Record &r);
        friend ostream &operator << (ostream &out,const Record &r);
        void display() const;
    };
    struct RecordNode{
        Record data;
        RecordNode *next; // 修正: *next
        RecordNode(const Record &r) : data(r), next(nullptr) {}
    };
    class RecordLinkedList {
    private:
        RecordNode *head; // 修正: *head
        RecordNode *tail;
        int count;
    public:
        RecordLinkedList() : head(nullptr), tail(nullptr), count(0) {}
        ~RecordLinkedList() { clear(); }
        // 深拷贝构造函数，否则链表复制会崩溃
        RecordLinkedList(const RecordLinkedList& other);
        RecordLinkedList& operator=(const RecordLinkedList& other);

        void clear();
        void append(const Record &record);
        int getCount() const { return count; }
        // 方便输出 JSON 数组
        string toJsonString() const;
        friend ostream &operator<<(ostream &out, const RecordLinkedList &list);
    };
    RecordLinkedList recordLinkedList;
public:
    using BaseHistory::BaseHistory;
    History(const History& other) : BaseHistory(other), recordLinkedList(other.recordLinkedList) {}
    History& operator=(const History& other) {
        if (this != &other) {
            BaseHistory::operator=(other);
            recordLinkedList = other.recordLinkedList;
        }
        return *this;
    }
    History() : BaseHistory() {}
    History(const BaseHistory& bh):BaseHistory(bh){}
    BaseHistory getBase()const{ return *this;}
    //BaseHistory getBase()const{return BaseHistory(hid,bid,rid,bname,rname,note,brtime,btime,rrtime,rtime);}

    void reqBorrow();
    void agrBorrow();
    void canBorrow();
    void refBorrow();
    void reqReturn();
    void agrReturn();
    void canReturn();
    void refReturn();

    //friend istream &operator >> (istream &in,Record &r);
    //friend ostream &operator << (ostream &out,const Record &r);
    friend istream &operator >> (istream &in,History &h);
    friend ostream &operator << (ostream &out,const History &h);
    string toJson()const;
};
//这里进行的总记录的更新
class HistoryList{
    struct HistoryNode {
        History data; // 修正：存储完整的 History 对象
        HistoryNode *next;
        HistoryNode(const History &h) : data(h), next(nullptr) {}
    };
private:
    HistoryNode *head;
    HistoryNode *tail;
    int count;
    // 辅助函数
    HistoryNode *findNode(const string &hid) const;
    HistoryNode *findNode(int index) const; // 新增：按索引找
public:
    HistoryList() : head(nullptr), tail(nullptr), count(0) {}
    ~HistoryList() { clear(); }

    // 同样需要深拷贝构造函数
    HistoryList(const HistoryList& other);
    HistoryList& operator=(const HistoryList& other);

    void clear();
    void add(const History& h);
    void add(const BaseHistory& bh); // 新增：兼容 add(BaseHistory)

    void del(const string &hid);
    void del(const BaseHistory &bh);

    int size() const { return count; }
    //显示相关：
    int show() const;
    int showAll() const { return show(); }

    void show(const string &hid) const;
    string show(const int &num) const;

    // 查找相关
    void schHistory(const string &s) const;
    History &getByHid(const string &hid);
    BaseHistory getByIndex(int index); // 新增
    //恢复operator[]兼容性
    BaseHistory &operator[](const string &hid);
    BaseHistory &operator[](const int &index);
    BaseHistory*findByHid(const string& hid);
    string toJsonArray() const;
    friend istream& operator>>(istream& in, HistoryList& hl);
    friend ostream& operator<<(ostream& out, const HistoryList& hl);

// private:
//     HistoryNode *findNode(const string &hid);
};

#endif //HISTORY_H
