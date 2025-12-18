//
// Created by 28646 on 2025/12/17.
//
#ifndef READER_H
#define READER_H
#include"History.h"
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
    string getRid() const { return rid; }
    string getRname() const { return rname; }
    int getBorrowNum() const { return borrowNum; }
    bool match(const string &s) const;
    void show() const;		//将BaseReader存储的信息输出到屏幕上
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
    Reader() {}
    BaseReader getBase() const {return BaseReader(rid,rname,borrowNum);}
    bool rpasswdRight(const string &_rpasswd) const { return rpasswd==_rpasswd; }
    void changeRpasswd(const string &_rpasswd){ rpasswd=_rpasswd; }
    void reqBorrow(const BH &bh);
    void agrBorrow(const BH &bh);
    void canBorrow(const BH &bh);
    void reqReturn(const BH &bh);
    void refReturn(const BH &bh);
    int showBorrowReq() const ;
    int showBorrowHis() const ;
    int showReturnReq() const ;
    int showReturnHis() const ;		//将Reader的相关历史记录输出到屏幕上,并返回相应历史记录的总数
    string showBorrowReq(const int &num) const ;
    string showBorrowHis(const int &num) const ;
    string showReturnReq(const int &num) const ;
    string showReturnHis(const int &num) const ;		//将特点序号的历史记录输出到屏幕上,并返回hid
    friend istream &operator >> (istream &in,Reader &r);
    friend ostream &operator << (ostream &out,const Reader &r);
};
class ReaderList{
    struct BaseReaderNode{
        BaseReader data;
        BaseReaderNode *next;

        BaseReaderNode(const BaseReader &br):data(br),next(nullptr){};
    };
private:
    BaseReaderNode *head;
    BaseReaderNode *tail;
    int count;
public:
    ReaderList():head(nullptr),tail(nullptr),count(0){}
    ~ReaderList(){
        clear();
    };
    ReaderList(const ReaderList&) = delete;
    ReaderList& operator=(const ReaderList&) = delete;
    void clear();
    int size() const{ return  count;}
    bool ridExist(const string &rid) const;
    void add(const BaseReader &br);
    void del(const BaseReader &br);
    void del(const string &id);
    int show() const;		//将rl存储的信息输出到屏幕上并返回rl的大小
    string show(const int &num) const;		//将rl[num]输出到屏幕上,并返回rid
    void show(const string &rid) const;
    void schReader(const string &s) const;
    friend istream &operator >> (istream &in,ReaderList &rl);
    friend ostream &operator << (ostream &out,const ReaderList &rl);
    string getRname(const string &rid) const ;
};
#endif //READER_H

