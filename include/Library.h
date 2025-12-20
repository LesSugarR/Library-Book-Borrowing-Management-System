//
// Created by LesSugarR on 25-12-10.
//
// 存数据（图书、读者、借阅记录/申请），三类操作权限（所有人查、管理员增删、读者借还查自己），管数据存文件

#ifndef LIBRARY_H
#define LIBRARY_H

#include "../include/Book.h"
#include "../include/Reader.h"
#include "../include/History.h"
#include "../include/Console.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <ctime>

// 全局控制台对象声明
extern Console con;

// 简化别名
using Book = BaseBook;
using BH = BaseHistory;

// 图书馆核心管理类
class Library {
private:
    class Data { 
    public:
        BookList bl;
        ReaderList rl;
        HistoryList hl;
        HistoryList borrowReq;
        HistoryList returnReq;

        // 生成历史记录ID
        std::string getHid() const { 
            return "His" + std::to_string(100000000 + hl.size()); 
        }
        
        // 数据持久化
        void outbl() const { con.outf("data/bookls", bl); }
        void outrl() const { con.outf("data/readerls", rl); }
        void outhl() const { con.outf("data/historyls", hl); }
        void outbr() const { con.outf("data/borrowReq", borrowReq); }
        void outrr() const { con.outf("data/returnReq", returnReq); }

        // 构造函数
        Data() { 
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
        
        // 批量查询
        int showBook() const;
        int showReader() const;
        int showHistory() const;
        
        // 按序号查询
        std::string showBook(int num) const;
        std::string showReader(int num) const;
        std::string showHistory(int num) const;
       
        // 按ID查询
        void showBook(const std::string& bid) const;
        void showReader(const std::string& rid) const;
        void showHistory(const std::string& hid) const;
        
        // 搜索
        void schBook(const std::string& s) const;
        
        // 存在性检查
        bool bidExist(const std::string& bid) const;
        bool ridExist(const std::string& rid) const;
    } op;

    // 管理员操作类
    class ManagerOperation {
        Data* data;
        std::string mpasswd;
        
    public:
        ManagerOperation(Data* _data) : data(_data) {
            con.inf("data/managerPasswd", mpasswd);
        }
        
        // 密码管理
        void outp() const { con.outf("data/managerPasswd", mpasswd); }
        
        // 图书管理
        void addBook(const std::string& bid, int num) const;
        void addBook(const Book& b) const;
        bool canDelb(const std::string& bid) const;
        bool canDelb(int num) const;
        void delBook(const std::string& bid) const;
        
        // 读者管理
        void addReader(const Reader& r) const;
        bool canDelr(const std::string& rid) const;
        bool canDelr(int num) const;
        void delReader(const std::string& rid) const;

        //修改信息
        void updateBookInfo(const std::string& bid, 
        const std::string& newBname = "",
        const std::string& newAuthor = "",
        const std::string& newPress = "",
        int newTotalNum = -1,
        int newBorrowNum = -1) const;
        
        // 审批申请
        void agrBorrow(const std::string& hid) const;
        void agrReturn(const std::string& hid) const;
        void refBorrow(const std::string& hid) const;
        void refReturn(const std::string& hid) const;
        
        // 申请查询
        int showBorrowReq() const;
        int showReturnReq() const;
        std::string showBorrowReq(int num) const;
        std::string showReturnReq(int num) const;
       
        // 权限校验
        bool canBorrow(const std::string& hid) const;
        bool mpasswdRight(const std::string& pwd) const { return mpasswd == pwd; }
        void changeMpasswd(const std::string& pwd) { mpasswd = pwd; outp(); }
        
        // 高级搜索
        void schReader(const std::string& s) const;
        void schHistory(const std::string& s) const;
    } mop;

    // 读者操作类
    class ReaderOperation {
        Data* data;
        std::string rid, rp;
        Reader r;
        
    public:
        ReaderOperation(Data* _data) : data(_data) {}
        
        // 身份管理
        void setReader(const std::string& _rid);
        
        // 借还申请
        void reqBorrow(const std::string& bid);
        void canBorrow(const std::string& hid);
        void reqReturn(const std::string& hid);
        void canReturn(const std::string& hid);

        // 个人记录查询
        int showBorrowHis() const;
        int showBorrowReq() const;
        int showReturnReq() const;
        int showReturnHis() const;

        // 按序号查询
        std::string showBorrowReq(int num) const;
        std::string showBorrowHis(int num) const;
        std::string showReturnReq(int num) const;
        std::string showReturnHis(int num) const;

        // 密码管理
        bool rpasswdRight(const std::string& rpasswd) const;
        void changeRpasswd(const std::string& rpasswd);
    } rop;
    
    // 构造函数
    Library() : op(&data), mop(&data), rop(&data) {}
    
    // 析构函数
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