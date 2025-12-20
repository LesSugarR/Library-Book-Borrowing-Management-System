//
// Created by LesSugarR on 25-12-10.
//
#include "../include/Book.h"
#include "../include/Reader.h"
#include "../include/History.h"
#include "../include/Library.h"
#include "../include/Console.h"
#include "../include/Functions.h"
#include <bits/stdc++.h>
#define LR Library::ReaderOperation
#define BH BaseHistory
using namespace std;
extern Console con;
string LR::addHistory(const string &bid) const {
    string  hid=data->getHid();
    string  rname=data->rl.getRname(rid);
    if (rname.empty()){
        return "";
    }
    string  bname= data->bl.getBname(bid);
    if (bname.empty()){
        return "";
    }
    History history(hid,bid,rid,bname,rname);
    string  p=con.hdir(hid);
    con.mkdir(p);
    p+=hid;
    con.outf(p,history);
    data->hl.add(history.getBase());
    data->outhl();
    return hid;
}
void LR::setReader(const string &_rid){
    rid=_rid;
    rp=con.rdir(rid)+rid;
    con.inf(rp,r);
}
void LR::reqBorrow(const string &bid){
    string hid = addHistory(bid);
    if (hid.empty()) return;

    // 使用findByHid方法查找历史记录
    BaseHistory* bhPtr = data->hl.findByHid(hid);
    if (bhPtr == nullptr) {
        cout << "错误：历史记录 " << hid << " 不存在！" << endl;
        return;
    }

    // 获取图书和历史记录信息
    string bp = con.bdir(bid) + bid;
    Book b;
    con.inf(bp, b);

    string hp = con.hdir(hid) + hid;
    History h;
    con.inf(hp, h);

    // 更新状态
    h.reqBorrow();
    con.outf(hp, h);
    BaseHistory bh = h.getBase();

    // 更新图书和读者
    b.reqBorrow(bh);
    con.outf(bp, b);

    r.reqBorrow(bh);
    con.outf(rp, r);

    // 更新链表：先删除旧的，再添加新的
    data->hl.del(hid);
    data->hl.add(bh);
    data->outhl();

    // 添加到借阅请求链表
    data->borrowReq.add(bh);
    data->outbr();
}
void LR::canBorrow(const string &hid) {
    BaseHistory* bhPtr = data->hl.findByHid(hid);
    if (bhPtr == nullptr) {
        cout << "错误：历史记录 " << hid << " 不存在！" << endl;
        return;
    }

    string bid = bhPtr->getBid();
    string bp = con.bdir(bid) + bid;
    Book b;
    con.inf(bp, b);

    string hp = con.hdir(hid) + hid;
    History h;
    con.inf(hp, h);

    h.canBorrow();
    con.outf(hp, h);
    BaseHistory bh = h.getBase();

    //?
    b.BaseBook::canBorrow();
    con.outf(bp, b);

    r.canBorrow(bh);
    con.outf(rp, r);

    // 更新历史记录链表
    data->hl.del(hid);
    data->hl.add(bh);
    data->outhl();

    // 从借阅请求链表中删除
    data->borrowReq.del(hid);
    data->outbr();
}
void LR::reqReturn(const string &hid) {
    BaseHistory* bhPtr = data->hl.findByHid(hid);
    if (bhPtr == nullptr) {
        cout << "错误：历史记录 " << hid << " 不存在！" << endl;
        return;
    }

    string bid = bhPtr->getBid();
    string bp = con.bdir(bid) + bid;
    Book b;
    con.inf(bp, b);

    string hp = con.hdir(hid) + hid;
    History h;
    con.inf(hp, h);

    h.reqReturn();
    con.outf(hp, h);
    BaseHistory bh = h.getBase();

    b.reqReturn(bh);
    con.outf(bp, b);

    r.reqReturn(bh);
    con.outf(rp, r);

    // 更新历史记录链表
    data->hl.del(hid);
    data->hl.add(bh);
    data->outhl();

    // 添加到归还请求链表
    data->returnReq.add(bh);
    data->outrr();
}
void LR::canReturn(const string &hid) {
    BaseHistory* bhPtr = data->hl.findByHid(hid);
    if (bhPtr == nullptr) {
        cout << "错误：历史记录 " << hid << " 不存在！" << endl;
        return;
    }

    string bid = bhPtr->getBid();
    string bp = con.bdir(bid) + bid;
    Book b;
    con.inf(bp, b);

    string hp = con.hdir(hid) + hid;
    History h;
    con.inf(hp, h);

    h.canReturn();
    con.outf(hp, h);
    BaseHistory bh = h.getBase();

    b.canReturn(bh);
    con.outf(bp, b);

    r.canReturn(bh);
    con.outf(rp, r);

    // 更新历史记录链表
    data->hl.del(hid);
    data->hl.add(bh);
    data->outhl();

    // 从归还请求链表中删除（使用BaseHistory对象）
    data->returnReq.del(bh);
    data->outrr();
}
int LR::showBorrowReq() const {
    return r.showBorrowReq();
}
int LR::showBorrowHis() const {
    return r.showBorrowHis();
}
int LR::showReturnReq() const {
    return r.showReturnReq();
}
int LR::showReturnHis() const {
    return r.showReturnHis();
}
string LR::showBorrowReq(const int &num) const {
    return r.showBorrowReq(num);
}
string LR::showBorrowHis(const int &num) const {
    return r.showBorrowHis(num);
}
string LR::showReturnReq(const int &num) const {
    return r.showReturnReq(num);
}
string LR::showReturnHis(const int &num) const {
    return r.showReturnHis(num);
}
void LR::changeRpasswd(const string &rpasswd){
    r.changeRpasswd(rpasswd);
    con.outf(rp,r);
}