//
// Created by LesSugarR on 25-12-10.
//


// Library_manager.cpp
// 图书管理功能（上架、下架、信息修改）

#include "../include/Library.h"
#include "../include/Console.h"
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

extern Console con;

// 简化别名
#define LM Library::ManagerOperation

// 图书管理

// 1. 增加库存
void LM::addBook(const string &bid, int num) const {  
    if (!data->bl.bidExist(bid)) {
        throw runtime_error("图书ID不存在");
    }
    
    BaseBook& book = data->bl[bid];
    int newTotal = book.getTotalNum() + num;
    
    if (newTotal < book.getBorrowNum()) {
        throw runtime_error("增加后总数量不能小于已借出数量");
    }
    
    book.setTotalNum(newTotal);
    data->outbl();
}

// 2. 新增图书
void LM::addBook(const Book &b) const {
    if (data->bl.bidExist(b.getBid())) {
        throw runtime_error("图书ID已存在");
    }
    
    if (b.getTotalNum() < 0) throw runtime_error("总数量不能为负数");
    if (b.getBorrowNum() < 0) throw runtime_error("借出数量不能为负数");
    if (b.getBorrowNum() > b.getTotalNum()) {
        throw runtime_error("借出数量不能超过总数量");
    }
    
    data->bl.add(b.getBase());
    data->outbl();
}

// 3. 检查图书是否可删除
bool LM::canDelb(const string &bid) const {
    if (!data->bl.bidExist(bid)) {
        return false;
    }
    return data->bl[bid].canDel();
}

bool LM::canDelb(int num) const { 
    if (num < 0 || num >= data->bl.size()) {
        return false;
    }
    return data->bl[num].canDel();
}

// 4. 删除图书
void LM::delBook(const string &bid) const {
    if (!data->bl.bidExist(bid)) {
        throw runtime_error("图书ID不存在");
    }
    
    if (!data->bl[bid].canDel()) {
        throw runtime_error("图书有借出记录，不可删除");
    }
    
    data->bl.del(bid);
    data->outbl();
}

// 5. 信息修改
void LM::updateBookInfo(const string &bid, 
                       const string &newBname,
                       const string &newAuthor,
                       const string &newPress,
                       int newTotalNum,
                       int newBorrowNum) const {
    if (!data->bl.bidExist(bid)) {
        throw runtime_error("图书ID不存在");
    }
    
    BaseBook& book = data->bl[bid];
    
    if (!newBname.empty()) book.setBname(newBname);
    if (!newAuthor.empty()) book.setAuthor(newAuthor);
    if (!newPress.empty()) book.setPress(newPress);
    
    if (newTotalNum >= 0) {
        if (newTotalNum < book.getBorrowNum()) {
            throw runtime_error("总数量不能小于已借出数量");
        }
        book.setTotalNum(newTotalNum);
    }
    
    if (newBorrowNum >= 0) {
        if (newBorrowNum > book.getTotalNum()) {
            throw runtime_error("借出数量不能超过总数量");
        }
        book.setBorrowNum(newBorrowNum);
    }
    
    data->outbl();
}

// 6. 密码管理
void LM::outp() const {
    con.outf("data/managerPasswd", mpasswd);
}

bool LM::mpasswdRight(const string &pwd) const {
    return mpasswd == pwd;
}

void LM::changeMpasswd(const string &pwd) {
    mpasswd = pwd;
    outp();
}

