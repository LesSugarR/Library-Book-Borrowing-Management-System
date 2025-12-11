//
// Created by LesSugarR on 25-12-10.
//

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
    BaseHistory(){}
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
    void display() const;
    friend istream &operator >> (istream &in,BaseHistory &bh);
    friend ostream &operator << (ostream &out,const BaseHistory &bh);
};

class History:public BaseHistory{
    //以下全部为私有
    class Record{
    protected:
        string time,note;
    public:
        Record(string _time,string _note):time(_time),note(_note){}
        Record(){}
        friend istream &operator>> (istream &in,Record &r);
        friend ostream  &operator<<(ostream &ostream1,const Record &r);
        void display() const;
    };
    struct RecordNode{
        Record data;//数据部分
        RecordNode *next;

        RecordNode(const Record &r): data(r),next(nullptr){}
        RecordNode() : data("", ""), next(nullptr) {}

        //展示每个节点
        void display() const{
           data.display();
        };
    };

    class RecordLinkedList{
    private:
        RecordNode *head;
        RecordNode *tail;
        int count;
    public:
        RecordLinkedList():head(nullptr),tail(nullptr),count(0){}
        ~RecordLinkedList(){
            clear();
        }
        void clear(){
            while (head != nullptr) {
                RecordNode* temp = head;
                head = head->next;
                delete temp;
            }
            tail = nullptr;
            count = 0;
        }
        //尾插法插入记录，这里记录是线性的
        void append(const Record&record){
            RecordNode* newNode = new RecordNode(record);
            if (tail == nullptr) {
                head = tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
            ++count;
        }
        //获得这个人的总记录
        int getCount() const {return count;}
        friend ostream  &operator<<(ostream &out,const RecordLinkedList &list){
            History::RecordNode* curr = list.head;
            while (curr != nullptr) {
                out << curr->data;
                if (curr->next != nullptr) out << " ";
                curr = curr->next;
            }
            return out;
        }

    };
    RecordLinkedList recordLinkedList;

public:
    using BaseHistory::BaseHistory;
    BaseHistory getBase()const{return BaseHistory(hid,bid,rid,bname,rname,note,brtime,btime,rrtime,rtime);}
    void reqBorrow();
    void agrBorrow();
    void canBorrow();
    void refBorrow();
    void reqReturn();
    void agrReturn();
    void canReturn();
    void refReturn();
    //这里需要输入吗？
    friend istream &operator >> (istream &in,Record &r);
    friend ostream &operator << (ostream &out,const Record &r);
    friend istream &operator >> (istream &in,History &h);
    friend ostream &operator << (ostream &out,const History &h);
};
//这里进行的总记录的更新
class HistoryList{
    struct BaseHistoryNode{
        BaseHistory data;
        BaseHistoryNode *next;

        BaseHistoryNode(const BaseHistory&bh): data(bh),next(nullptr){}

    };
private :
    BaseHistoryNode *head;
    BaseHistoryNode *tail;
    int count;
public:
    HistoryList():head(nullptr),tail(nullptr),count(0){}
    ~HistoryList(){
        clear();
    }
    HistoryList(const HistoryList&) = delete;
    HistoryList& operator=(const HistoryList&) = delete;
    void clear();
    void add(const BaseHistory& bh);
    void showAll() const;
    BaseHistory* findByHid(const std::string& hid);
    int size() const { return count; }

    friend std::istream& operator>>(std::istream& in, HistoryList& hl);
    friend std::ostream& operator<<(std::ostream& out, const HistoryList& hl);

};

#endif //HISTORY_H
