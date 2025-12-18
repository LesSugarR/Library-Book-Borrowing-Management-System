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
#include <stdexcept>
#include <ctime>

//全局控制台对象声明
extern Console con;

//图书馆核心管理类（整体功能入口）
class Library{
    //异常处理+封装
    class Data{ 
    public:
        BookList bl;             //关联book.h中booklist
        ReaderList rl;           //关联reader.h中readerlist
        HistoryList hl;          //借阅历史列表（存储所有已经审批的借还记录）
        HistoryList borrowReq;   //借阅申请列表（存储待管理员审批的借阅请求）
        HistoryList returnReq;   //还书申请列表（存储待管理员审批的还书请求）

        //生成唯一历史记录ID
        string getHid() const { return "His"+to_string(100000000+hl.size()); }//拼接“His”+8位数字
        //数据持久化方法，将内存数据写入本地文件，重启不丢
        void outbl() const { con.outf("data/bookls",bl); }    //保存列表到这些文件
        void outrl() const { con.outf("data/readerls",rl);}
        void outhl() const { con.outf("data/historyls",hl); }
        void outbr() const { con.outf("data/borrowReq",borrowReq); }
        void outrr() const { con.outf("data/returnReq",returnReq); }

        //Data构造函数（程序启动时从文件加载数据到内存）
        Data(){ 
            //调用Console的inf方法，从本地文件读取数据到对应列表（顺序与保存一致）
            con.inf("data/bookls",bl);            //加载图书
            con.inf("data/readerls",rl);          //加载读者
            con.inf("data/historyls",hl);         //加载历史记录
            con.inf("data/borrowReq",borrowReq);  //加载未审批借阅
            con.inf("data/returnReq",returnReq);  //加载未审批还书
        }
    }data; //library类的核心数据对象

public:
    //公共操作类，读者和管理员都能使用此基础查询功能，无数据修改权限
    class Operation{
        Data *data; 
    public:
        Operation(Data *_data):data(_data){}
        //1.批量查询
        int showBook() const;              //显示所有图书，返回图书总数
        int showReader() const;            //显示......., 返回读者总数
        int showHistory() const;           //..........., 返回历史记录总数
        
        //2.按序号查询（返回对应ID）
        string showBook(const int &num) const;    //显示第num本图书，返回图书的bid
        string showReader(const int &num) const;  //显示第num个读者，返回读者的rid
        string showHistory(const int &num) const; //显示第num条记录，返回history的hid
       
        //3.按ID精准查询（查看单个对象的详细信息）
        void showBook(const string &bid) const;     //按bid显示某本书详情
        void showReader(const string &rid) const;   //按rid显示某读者
        void showHistory(const string &hid) const;  //按hid显示单条历史记录
        
        //4.搜索功能（有模糊匹配）
        void schBook(const string &s) const;    //按书名或作者搜索图书
        
        //5.存在性校验（防止操作不存在的对象）
        bool bidExist(const string &bid);  //检验该bid是否存在
        bool ridExist(const string &rid);  //检验该rid是否存在
    }op;

    //管理员专属的高权限操作（增删图书和读者、批阅申请）
    class ManagerOperation{
        Data *data;      //通过指针直接操作修改所有数据
        string mpasswd;  //管理员密码，存储在data/managerPasswd文件（还没写到这里）
    public:
        //绑定数据+加载管理员密码（从本地文件读取密码）
        ManagerOperation(Data *_data):data(_data){
            con.inf("data/managerPasswd",mpasswd);	//从文件加载密码到mpasswd
        }
        //密码持久化：修改后同步保存到文件（重启不丢）
        void outp() const { con.outf("data/managerPasswd",mpasswd); }
        //1.图书管理功能（增、删、库存调整）
        void addBook(const string &bid,const int &num) const;//按bid增加库存
        void addBook(const Book &b) const;       //新增一本图书
        bool canDelb(const string &bid) const;   //检验图书是否可删（无借出的图书可删）
        bool canDelb(const int &num) const;      //按序号检验图书是否可删
        void delBook(const string &bid) const;   //按bid删除图书（需要先通过canDelb校验）
        
        //2.读者管理（增/删）
        void addReader(const Reader &r) const;   //新增一个读者
        bool canDelr(const string &rid) const;   //检验读者是否可删（无未还图书可删）
        bool canDelr(const int &num) const;      //按序号校验读者是否可删
        void delReader(const string &rid) const; //按rid删除读者（需要通过canDelr检验才行）
        
        //3.审批申请功能(处理读者的借还请求)
        void agrBorrow(const string &hid) const; //同意借阅申请
        void agrReturn(const string &hid) const; //同意还书申请
        void refBorrow(const string &hid) const; //拒绝借阅申请
        void refReturn(const string &hid) const; //拒绝还书申请
        
        //4.申请列表查询（查看待审批的请求）
        int showBorrowReq() const;     //显示所有借阅申请，返回申请总数
        int showReturnReq() const;     //显示所有还书申请，返回总数
        string showBorrowReq(const int &num) const;    //按序号显示借阅申请，返回hid
        string showReturnReq(const int &num) const;    //按序号显示还书申请，也返回hid
       
        //5.管理员权限校验
        bool canBorrow(const string &hid) const;    //检验借阅符不符合条件
        bool mpasswdRight(const string &_mpasswd) const { return mpasswd==_mpasswd; } //密码验证
        void changeMpasswd(const string &_mpasswd){ mpasswd=_mpasswd; outp(); }  //修改管理员密码
        
        //6.高级搜索功能（管理员的：查询读者/历史记录）
        void schReader(const string &s) const;  //通过id或姓名搜索读者
        void schHistory(const string &s) const; //通过hid/bid/rid搜索历史记录
    }mop;

    //读者操作类（借还书申请、个人纪录查询，权限只能操作自己的数据）
    class ReaderOperation{
        Data *data;
        string rid,rp;
        Reader r;   //存储详情，姓名、密码、已借数量
        string addHistory(const string &bid) const;
    public:
        ReaderOperation(Data *_data):data(_data){}
        //1.读者身份管理
        void setReader(const string &_rid);  //按rid加载读者信息到r
        
        //2.借还申请功能（需要先登录）
        void reqBorrow(const string &bid);  //发起借阅（按图书bid）
        void canBorrow(const string &hid);  //取消借阅（按申请id）
        void reqReturn(const string &hid);  //发起还书
        void canReturn(const string &hid);  //取消还书

        //3.个人记录查询（保护隐私）
        int showBorrowHis() const;    //借阅历史，返回总数
        int showBorrowReq() const;    //未审批借阅申请
        int showReturnReq() const;    //未审批还书
        int showReturnHis() const;    //还书历史

        //4.按序号查询个人纪录（返回id来取消申请）
        string showBorrowReq(const int &num) const;   //按序号显示自己的解约申请，返回hid
        string showBorrowHis(const int &num) const;
        string showReturnReq(const int &num) const;
        string showReturnHis(const int &num) const;

        //5.读者密码管理（验证+修改）
        bool rpasswdRight(const string &rpasswd) const { return r.rpasswdRight(rpasswd); }
        void changeRpasswd(const string &rpasswd);
    }rop;

    
    Library():op(Operation(&data)),mop(ManagerOperation(&data)),rop(ReaderOperation(&data)){}
};

#endif
