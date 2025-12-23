#ifndef READER_H
#define READER_H
#include"History.h"
#include "Book.h"
#include<bits/stdc++.h>
#define HL HistoryList
#define BH BaseHistory
using namespace std;

class BaseReader{
protected:
    string  rid,rname;
    int borrowNum;
public:
    BaseReader(string _rid,string _rname,int _borrowNum=0):rid(_rid),rname(_rname),borrowNum(_borrowNum){}
    BaseReader(){}
    virtual ~BaseReader() {} // 添加虚析构函数
    virtual string toJson() const;

    string getRid() const { return rid; }
    string getRname() const { return rname; }
    int getBorrowNum() const { return borrowNum; }
    bool match(const string &s) const;
    void show() const;		//将BaseReader存储的信息输出到屏幕上
    bool canDel() const { return borrowNum==0; }

    friend istream &operator >> (istream &in,BaseReader &br);
    friend ostream &operator << (ostream &out,const BaseReader &br);
};
class Reader : public BaseReader {
    string rpasswd;
    HL borrowReq;
    HL borrowHis;
    HL returnReq;
    HL returnHis;
public:
    Reader(string _rid, string _rname, string _rpasswd, int _borrowNum = 0)
            : BaseReader(_rid, _rname, _borrowNum) { rpasswd = _rpasswd; }
    Reader() : BaseReader() {}
    const HL& getBorrowHis() const { return borrowHis; }
    HL& getBorrowHis() { return borrowHis; }
    BaseReader getBase() const {
        return *this; // 直接把自己返回去，编译器会自动切片，只保留 BaseReader 的部分
    }
    string getPwd() const { return rpasswd; }
    bool rpasswdRight(const string &_rpasswd) const { return rpasswd==_rpasswd; }
    void changeRpasswd(const string &_rpasswd){ rpasswd=_rpasswd; }
    void reqBorrow(const BH &bh);
    void agrBorrow(const BH &bh);
    void canBorrow(const BH &bh);
    void refBorrow(const BH &bh);
    void reqReturn(const BH &bh);
    void agrReturn(const BH &bh);
    void canReturn(const BH &bh);
    void refReturn(const BH &bh);
    void setRid(const string &_rid){rid = _rid;};
    void setRname(const string &_rname){rname = _rname;};
    void setBorrowNum(int _borrowNum){borrowNum = _borrowNum;};
    int showBorrowReq() const ;
    int showBorrowHis() const ;
    int showReturnReq() const ;
    int showReturnHis() const ;		//将Reader的相关历史记录输出到屏幕上,并返回相应历史记录的总数
    string showBorrowReq(const int &num) const ;
    string showBorrowHis(const int &num) const ;
    string showReturnReq(const int &num) const ;
    string showReturnHis(const int &num) const ;		//将特点序号的历史记录输出到屏幕上,并返回hid
    virtual string toJson() const;
    friend istream &operator >> (istream &in,Reader &r);
    friend ostream &operator << (ostream &out,const Reader &r);

};
class ReaderList{
    struct BaseReaderNode{
        Reader data; // 修改：存储完整的 Reader 对象，防止对象切割
        BaseReaderNode *next;
        BaseReaderNode(const Reader &r) : data(r), next(nullptr) {};
    };
private:
    BaseReaderNode *head;
    BaseReaderNode *tail;
    int count;
    BaseReaderNode *findNode(const string &rid); // 辅助函数
public:
    ReaderList():head(nullptr),tail(nullptr),count(0){}
    ~ReaderList(){clear();};
    Reader& getByRid(const string &rid);
    Reader &getByIndex(const int &index) const;
    friend istream &operator>>(istream &in, ReaderList &rl);
    friend ostream &operator<<(ostream &out, const ReaderList &rl);
    ReaderList(const ReaderList&) = delete;
    ReaderList& operator=(const ReaderList&) = delete;
    void clear();
    int size() const{ return  count;}
    bool ridExist(const string &rid) const;

    void add(const Reader &r);
    void del(const BaseReader &br);//兼容旧接口
    void del(const string &id);

    int show() const;		//将rl存储的信息输出到屏幕上并返回rl的大小
    string show(const int &num) const;		//将rl[num]输出到屏幕上,并返回rid
    void show(const string &rid) const;
    void schReader(const string &s) const;

    string toJsonString() const;
    string getRname(const string &rid) const ;

};



#endif //READER_H

