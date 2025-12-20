//
// Created by LesSugarR on 25-12-10.
//

//存数据（图书、读者、借阅记录/申请），三类操作权限（所有人查、管理员增删、读者借还查自己），管数据存文件

#ifndef LIBRARY_H
#define LIBRARY_H

#include "../include/Book.h"
#include "../include/Reader.h"    
#include "../include/History.h"   
#include "../include/Console.h"
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <ctime>

// 全局控制台对象声明
extern Console con;

// 图书馆核心管理类
class Library {
    class Data {
    public:
        BookList bl;
        ReaderList rl;           // 
        HistoryList hl;          // 
        HistoryList borrowReq;   // 
        HistoryList returnReq;   // 

        // 生成唯一历史记录ID
        std::string getHid() const {
            static int counter = 0;
            return "His" + std::to_string(100000000 + hl.size() + ++counter);
        }

        // 数据持久化
        void outbl() const { con.outf("data/bookls", bl); }
        void outrl() const { con.outf("data/readerls", rl); }
        void outhl() const { con.outf("data/historyls", hl); }
        void outbr() const { con.outf("data/borrowReq", borrowReq); }
        void outrr() const { con.outf("data/returnReq", returnReq); }

        // 文件存在性检查
        void checkFileExist(const std::string& filePath) const {
            std::ifstream file(filePath);
            if (!file.is_open()) {
                std::ofstream newFile(filePath);
                newFile.close();
            }
        }

        // 构造函数
        Data() {
            checkFileExist("data/bookls");
            checkFileExist("data/readerls");
            checkFileExist("data/historyls");
            checkFileExist("data/borrowReq");
            checkFileExist("data/returnReq");
            checkFileExist("data/managerPasswd");
            
            con.inf("data/bookls", bl);
            con.inf("data/readerls", rl);
            con.inf("data/historyls", hl);
            con.inf("data/borrowReq", borrowReq);
            con.inf("data/returnReq", returnReq);
        }
    } data;

public:
    // 公共操作类
    class Operation {
        Data* data;
    public:
        Operation(Data* _data) : data(_data) {}

        // 图书查询
        int showBook() const { return data->bl.showAll(); }  
        std::string showBook(int num) const { return data->bl.show(num); }
        void showBook(const std::string& bid) const { data->bl.show(bid); }
        void schBook(const std::string& s) const { data->bl.schBook(s); }
        bool bidExist(const std::string& bid) const { return data->bl.bidExists(bid); }

        // 读者查询
        int showReader() const { return data->rl.show(); }  
        std::string showReader(int num) const { return data->rl.show(num); }
        void showReader(const std::string& rid) const { data->rl.show(rid); }
        bool ridExist(const std::string& rid) const { return data->rl.ridExist(rid); }

        // 历史查询
        int showHistory() const { return data->hl.showAll(); }  
        std::string showHistory(int num) const { return data->hl.show(num); }
        void showHistory(const std::string& hid) const { data->hl.show(hid); }
    } op;

    // 管理员操作类
    class ManagerOperation {
        Data* data;
        std::string mpasswd;
    public:
        ManagerOperation(Data* _data) : data(_data) {
            con.inf("data/managerPasswd", mpasswd);
        }

        void outp() const { con.outf("data/managerPasswd", mpasswd); }

        // 图书管理
        void addBook(const std::string& bid, int num) const {
            // 实现库存增加
        }
        void addBook(const Book& b) const { data->bl.add(b); }
        bool canDelb(const std::string& bid) const {
            BaseBook* book = data->bl.findByBid(bid);
            return book && book->canDelete();
        }
        bool canDelb(int num) const { /* 实现 */ return false; }
        void delBook(const std::string& bid) const { data->bl.del(bid); }

        // 读者管理
        void addReader(const Reader& r) const { data->rl.add(r); }
        bool canDelr(const std::string& rid) const { /* 实现 */ return false; }
        bool canDelr(int num) const { /* 实现 */ return false; }
        void delReader(const std::string& rid) const { data->rl.del(rid); }

        // 审批申请
        void agrBorrow(const std::string& hid) const {
            BaseHistory* bh = data->borrowReq.findByHid(hid);
            if (bh) bh->agrBorrow();
        }
        void agrReturn(const std::string& hid) const {
            BaseHistory* bh = data->returnReq.findByHid(hid);
            if (bh) bh->agrReturn();
        }
        void refBorrow(const std::string& hid) const {
            BaseHistory* bh = data->borrowReq.findByHid(hid);
            if (bh) bh->refBorrow();
        }
        void refReturn(const std::string& hid) const {
            BaseHistory* bh = data->returnReq.findByHid(hid);
            if (bh) bh->refReturn();
        }

        // 申请查询
        int showBorrowReq() const { return data->borrowReq.showAll(); }
        int showReturnReq() const { return data->returnReq.showAll(); }
        std::string showBorrowReq(int num) const { return data->borrowReq.show(num); }
        std::string showReturnReq(int num) const { return data->returnReq.show(num); }

        bool canBorrow(const std::string& hid) const { /* 实现 */ return false; }
        bool mpasswdRight(const std::string& pwd) const { return mpasswd == pwd; }
        void changeMpasswd(const std::string& pwd) { mpasswd = pwd; outp(); }

        void schReader(const std::string& s) const { data->rl.schReader(s); }
        void schHistory(const std::string& s) const { data->hl.schHistory(s); }
    } mop;

    // 读者操作类
    class ReaderOperation {
        Data* data;
        std::string rid, rp;
        Reader* r;
    public:
        ReaderOperation(Data* _data) : data(_data), r(nullptr) {}

        void setReader(const std::string& _rid) {
            rid = _rid;
            // 从ReaderList中找到对应读者
        }

        // 借还申请
        void reqBorrow(const std::string& bid) {
            if (r) {
                BH bh;
                bh.bid = bid;
                bh.rid = rid;
                r->reqBorrow(bh);
            }
        }
        
        void canBorrow(const std::string& hid) {
            BaseHistory* bh = data->borrowReq.findByHid(hid);
            if (bh && bh->getRid() == rid) {
                bh->canBorrow();
            }
        }
        
        void reqReturn(const std::string& hid) {
            BaseHistory* bh = data->borrowReq.findByHid(hid);
            if (bh && bh->getRid() == rid) {
                r->reqReturn(*bh);
            }
        }
        
        void canReturn(const std::string& hid) {
            BaseHistory* bh = data->returnReq.findByHid(hid);
            if (bh && bh->getRid() == rid) {
                bh->canReturn();
            }
        }

        // 个人记录
        int showBorrowHis() const { return r ? r->showBorrowHis() : 0; }
        int showBorrowReq() const { return r ? r->showBorrowReq() : 0; }
        int showReturnReq() const { return r ? r->showReturnReq() : 0; }
        int showReturnHis() const { return r ? r->showReturnHis() : 0; }

        std::string showBorrowReq(int num) const { return r ? r->showBorrowReq(num) : ""; }
        std::string showBorrowHis(int num) const { return r ? r->showBorrowHis(num) : ""; }
        std::string showReturnReq(int num) const { return r ? r->showReturnReq(num) : ""; }
        std::string showReturnHis(int num) const { return r ? r->showReturnHis(num) : ""; }

        bool rpasswdRight(const std::string& pwd) const { return r && r->rpasswdRight(pwd); }
        void changeRpasswd(const std::string& pwd) { if (r) r->changeRpasswd(pwd); }
    } rop;

    // 构造函数/析构函数
    Library() : op(&data), mop(&data), rop(&data) {}
    
    ~Library() {
        data.outbl();
        data.outrl();
        data.outhl();
        data.outbr();
        data.outrr();
        mop.outp();
    }
};

#endif // LIBRARY_H