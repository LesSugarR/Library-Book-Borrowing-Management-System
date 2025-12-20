#include "../include/Book.h"
#include "../include/Reader.h"
#include "../include/History.h"
#include "../include/Library.h"
#include "../include/Console.h"
#include <bits/stdc++.h>
#define LM Library::ManagerOperation
#define BH BaseHistory
using namespace std;

//Library_manager.cpp
extern Console con;

void LM::addBook(const string &bid,const int &num) const {
    string p=con.bdir(bid)+bid;
    Book b; con.inf(p,b);
    b.addNum(num); con.outf(p,b);

    // 删除原记录并添加更新后的记录
    data->bl.del(bid);
    data->bl.add(b.getBase());
    data->outbl();
}

void LM::addBook(const Book &b) const {
    string p=con.bdir(b.getBid());
    con.mkdir(p);
    p+=b.getBid();
    con.outf(p,b);
    data->bl.add(b.getBase());
    data->outbl();
}

void LM::addReader(const Reader &r) const {
    string p=con.rdir(r.getRid());
    con.mkdir(p);
    p+=r.getRid();
    con.outf(p,r);
    data->rl.add(r.getBase());
    data->outrl();
}

bool LM::canDelb(const string &bid) const {
    BaseBook& book = data->bl.getByBid(bid);  // 修改这里
    return book.canDel();
}

bool LM::canDelr(const string &rid) const {
    BaseReader& reader = data->rl.getByRid(rid);  // 需要 ReaderList 有 getByRid 方法
    return reader.canDel();
}

bool LM::canDelb(const int &num) const {
    BaseBook& book = data->bl.getByIndex(num);  // 修改这里
    return book.canDel();
}

bool LM::canDelr(const int &num) const {
    BaseReader& reader = data->rl.getByIndex(num);  // 需要 ReaderList 有 getByIndex 方法
    return reader.canDel();
}

void LM::delBook(const string &bid) const {
    string p=con.bdir(bid);
    con.rmdir(p);
    data->bl.del(bid);
    data->outbl();
}

void LM::delReader(const string &rid) const {
    string p=con.rdir(rid);
    con.rmdir(p);
    data->rl.del(rid);
    data->outrl();
}

void LM::agrBorrow(const string &hid) const {
    // 注意：HistoryList 也需要类似修改
    BH bh = data->hl.getByHid(hid);  // 需要 HistoryList 有 getByHid 方法
    string bid=bh.getBid(),rid=bh.getRid();

    string bp=con.bdir(bid)+bid; Book b; con.inf(bp,b);
    string rp=con.rdir(rid)+rid; Reader r; con.inf(rp,r);
    string hp=con.hdir(hid)+hid; History h; con.inf(hp,h);

    h.agrBorrow(); con.outf(hp,h); bh=h.getBase();
    b.agrBorrow(bh); con.outf(bp,b);
    r.agrBorrow(bh); con.outf(rp,r);

    data->bl.del(bid); data->bl.add(b.getBase()); data->outbl();
    data->rl.del(rid); data->rl.add(r.getBase()); data->outrl();
    data->hl.del(hid); data->hl.add(bh); data->outhl();
    data->borrowReq.del(hid); data->outbr();
}

void LM::agrReturn(const string &hid) const {
    BH bh = data->hl.getByHid(hid);  // 需要 HistoryList 有 getByHid 方法
    string bid=bh.getBid(),rid=bh.getRid();

    string bp=con.bdir(bid)+bid; Book b; con.inf(bp,b);
    string rp=con.rdir(rid)+rid; Reader r; con.inf(rp,r);
    string hp=con.hdir(hid)+hid; History h; con.inf(hp,h);

    h.agrReturn(); con.outf(hp,h); bh=h.getBase();
    b.agrReturn(bh); con.outf(bp,b);
    r.agrReturn(bh); con.outf(rp,r);

    data->bl.del(bid); data->bl.add(b.getBase()); data->outbl();
    data->rl.del(rid); data->rl.add(r.getBase()); data->outrl();
    data->hl.del(hid); data->hl.add(bh); data->outhl();
    data->returnReq.del(hid); data->outrr();
}

void LM::refBorrow(const string &hid) const {
    BH bh = data->hl.getByHid(hid);  // 需要 HistoryList 有 getByHid 方法
    string bid=bh.getBid(),rid=bh.getRid();

    string bp=con.bdir(bid)+bid; Book b; con.inf(bp,b);
    string rp=con.rdir(rid)+rid; Reader r; con.inf(rp,r);
    string hp=con.hdir(hid)+hid; History h; con.inf(hp,h);

    h.refBorrow(); con.outf(hp,h); bh=h.getBase();
    b.refBorrow(bh); con.outf(bp,b);
    r.refBorrow(bh); con.outf(rp,r);

    data->hl.del(hid); data->hl.add(bh); data->outhl();
    data->borrowReq.del(hid); data->outbr();
}

void LM::refReturn(const string &hid) const {
    BH bh = data->hl.getByHid(hid);  // 需要 HistoryList 有 getByHid 方法
    string bid=bh.getBid(),rid=bh.getRid();

    string bp=con.bdir(bid)+bid; Book b; con.inf(bp,b);
    string rp=con.rdir(rid)+rid; Reader r; con.inf(rp,r);
    string hp=con.hdir(hid)+hid; History h; con.inf(hp,h);

    h.refReturn(); con.outf(hp,h); bh=h.getBase();
    b.refReturn(bh); con.outf(bp,b);
    r.refReturn(bh); con.outf(rp,r);

    data->hl.del(hid); data->hl.add(bh); data->outhl();
    data->returnReq.del(hid); data->outrr();
}

int LM::showBorrowReq() const {
    return data->borrowReq.showAll();
}

int LM::showReturnReq() const {
    return data->returnReq.showAll();
}

string LM::showBorrowReq(const int &num) const {
    return data->borrowReq.show(num);
}

string LM::showReturnReq(const int &num) const {
    return data->returnReq.show(num);
}

bool LM::canBorrow(const string &hid) const {
    BH bh = data->hl.getByHid(hid);
    string bid = bh.getBid();
    BaseBook& book = data->bl.getByBid(bid);  // 修改这里
    return book.canBorrow();
}

void LM::schReader(const string &s) const {
    data->rl.schReader(s);
}

void LM::schHistory(const string &s) const {
    data->hl.schHistory(s);
}