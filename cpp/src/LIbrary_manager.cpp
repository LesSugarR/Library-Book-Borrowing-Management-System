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
    Book b;
    con.inf(p,b);
    b.addNum(num);
    con.outf(p,b);
    // 删除原记录并添加更新后的记录
    data->bl.del(bid);
    data->bl.add(b);
    data->outbl();
}

void LM::addBook(const Book &b) const {
    string p = con.bdir(b.getBid());
    con.mkdir(p);
    p += b.getBid();
    con.outf(p, b);
    // 【修改】直接添加 b
    data->bl.add(b);
    data->outbl();
}

void LM::addReader(const Reader &r) const {
    string p=con.rdir(r.getRid());
    con.mkdir(p);
    p+=r.getRid();
    con.outf(p,r);
    data->rl.add(r);
    data->outrl();
}

bool LM::canDelb(const string &bid) const {
    return data->bl.getByBid(bid).canDel();
}

bool LM::canDelr(const string &rid) const {
    // 直接调用
    return data->rl.getByRid(rid).canDel();
}

bool LM::canDelb(const int &num) const {
    return data->bl.getByIndex(num).canDel();
}

bool LM::canDelr(const int &num) const {
    return data->rl.getByIndex(num).canDel();
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
    BH bh = data->hl.getByHid(hid);
    string bid = bh.getBid();
    string rid = bh.getRid();

    string bp = con.bdir(bid) + bid; Book b; con.inf(bp, b);
    string rp = con.rdir(rid) + rid; Reader r; con.inf(rp, r);
    string hp = con.hdir(hid) + hid; History h; con.inf(hp, h);

    h.agrBorrow(); con.outf(hp, h); bh = h.getBase();
    b.agrBorrow(bh); con.outf(bp, b);
    r.agrBorrow(bh); con.outf(rp, r);
    // 【关键修改】add 时直接传对象，去掉 .getBase()
    data->bl.del(bid); data->bl.add(b); data->outbl();
    data->rl.del(rid); data->rl.add(r); data->outrl(); // 这里最重要！
    data->hl.del(hid); data->hl.add(h); data->outhl();
    data->borrowReq.del(hid); data->outbr();
}

void LM::agrReturn(const string &hid) const {
    BH bh = data->hl.getByHid(hid);

    string bid = bh.getBid();
    string rid = bh.getRid();

    string bp = con.bdir(bid) + bid; Book b; con.inf(bp, b);
    string rp = con.rdir(rid) + rid; Reader r; con.inf(rp, r);
    string hp = con.hdir(hid) + hid; History h; con.inf(hp, h);

    h.agrReturn(); con.outf(hp, h); bh = h.getBase();
    b.agrReturn(bh); con.outf(bp, b);
    r.agrReturn(bh); con.outf(rp, r);

    // 【关键修改】去掉 .getBase()
    data->bl.del(bid); data->bl.add(b); data->outbl();
    data->rl.del(rid); data->rl.add(r); data->outrl();
    data->hl.del(hid); data->hl.add(h); data->outhl();

    data->returnReq.del(hid); data->outrr();
}

void LM::refBorrow(const string &hid) const {
    BH bh = data->hl.getByHid(hid);

    string bid = bh.getBid();
    string rid = bh.getRid();

    string bp = con.bdir(bid) + bid; Book b; con.inf(bp, b);
    string rp = con.rdir(rid) + rid; Reader r; con.inf(rp, r);
    string hp = con.hdir(hid) + hid; History h; con.inf(hp, h);

    h.refBorrow(); con.outf(hp, h); bh = h.getBase();
    b.refBorrow(bh); con.outf(bp, b);
    r.refBorrow(bh); con.outf(rp, r);

    data->hl.del(hid); data->hl.add(h); data->outhl();
    data->borrowReq.del(hid); data->outbr();
}

void LM::refReturn(const string &hid) const {
    BH bh = data->hl.getByHid(hid);

    string bid = bh.getBid();
    string rid = bh.getRid();

    string bp = con.bdir(bid) + bid; Book b; con.inf(bp, b);
    string rp = con.rdir(rid) + rid; Reader r; con.inf(rp, r);
    string hp = con.hdir(hid) + hid; History h; con.inf(hp, h);

    h.refReturn(); con.outf(hp, h); bh = h.getBase();
    b.refReturn(bh); con.outf(bp, b);
    r.refReturn(bh); con.outf(rp, r);

    data->hl.del(hid); data->hl.add(h); data->outhl();
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
    string bid = data->hl.getByHid(hid).getBid();
    return data->bl.getByBid(bid).canBorrow();
}

void LM::schReader(const string &s) const {
    data->rl.schReader(s);
}

void LM::schHistory(const string &s) const {
    data->hl.schHistory(s);
}