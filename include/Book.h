// Created by LesSugarR on 25-12-10.
//
//明确图书要存哪些信息（定义数据结构）


#ifndef BOOK_H
#define BOOK_H

#include"History.h"
#include<string>
#include<vector>
#include<iostream>
#include<stdexcept>
#include<ctime>

#define BH BaseHistory
#define HL HistoryList

namespace LibraryBookSystem{
struct BaseBook{
protected:
std::string bid,bname,author,press;
int totalNum,borrowNum;
std::string inTime;
public :
BaseBook(std::string _bid,std::string _bname,std::string _author,std::string _press,int _totalNum,int _borrowNum = 0, std::string _inTime = ""){
    bid = _bid;
    bname = _bname;
    author = _author;
    press = _press;
    totalNum = _totalNum;
    borrowNum = _borrowNum;
    inTime = _inTime.empty() ? getCurrentTime() : _inTime;
}
//无参构造函数
BaseBook(){
    bid = "";
    bname = "";
    author = "";
    press = "";
    totalNum = 0;
    borrowNum = 0;
    inTime = getCurrentTime();
}

static std::string getCurrentTime(){
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[20];
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", 
            1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday,
            ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
    return std::string(buf);
}
//访问器函数
std::string getInTime() const{
    return inTime;
}
std::string getBid() const{
    return bid;
}
std::string getBname() const{
    return bname;
}
std::string getAuthor() const{
    return author;
}
std::string getPress() const{
    return press;
}
int getBorrowNum() const{
    return borrowNum;
}
int getTotalNum() const{
    return totalNum;
}
//判断核心函数
bool canBorrow() const {
    return (totalNum - borrowNum)>0;
}
bool canDel() const {
    return borrowNum==0;
}
//成员函数声明
bool match(const std::string &s) const;
void show() const;
// 友元运算符声明（确保作用域正确，无拼写错误）
friend std::istream &operator >>(std::istream &in , BaseBook &book);
friend std::ostream &operator <<(std::ostream &out , const BaseBook &book);
};

// 2. Book：继承BaseBook（struct支持继承）
struct Book :public BaseBook{
private:
 HL borrowReq;
 HL borrowHis;
 HL returnReq;
 HL returnHis;
public:
//继承基类构造函数
 using BaseBook::BaseBook;
 //成员函数声明
 BaseBook getBase() const {
    return BaseBook(bid,bname,author,press,totalNum,borrowNum,inTime);
 }
 void addNum(const int &num);
 void reqBorrow(const BH &bh);
 void agrBorrow(const BH &bh);
 void canBorrow(const BH &bh);
 void refBorrow(const BH &bh);
 void reqReturn(const BH &bh);
 void agrReturn(const BH &bh);
 void canReturn(const BH &bh);
 void refReturn(const BH &bh);
 int showBorrowReq() const ;
 int showBorrowHis() const ;
 int showReturnReq() const ;
 int showReturnHis() const ;
 std::string showBorrowReq(const int &num) const;
 std::string showBorrowHis(const int &num) const;
 std::string showReturnReq(const int &num) const;
 std::string showReturnHis(const int &num) const;
 //友元运算符重载声明
 friend std::istream &operator >>(std::istream &in, Book &book);
 friend std::ostream &operator <<(std::ostream &out, const Book &book);
};

// 3. BookList
struct BookList{
    private:
    std::vector<BaseBook> bl;
    public:
     //重载【】运算符
    BaseBook &operator [] (const std::string &bid){
        for(size_t i=0;i<bl.size();++i)
        {
            if(bl[i].getBid()==bid){
                return bl[i];
            }
        }
        throw std::runtime_error("Bid does not exist!");
      }
      BaseBook &operator [] (const int &num){
        if(num<0||num>=(int)bl.size()){
            throw std::out_of_range("Index out of bounds!");
        }
        return bl[num];
      }
      //成员函数声明
      int size() const{
        return bl.size();
      }
      bool bidExist(const std::string &bid) const;
      void add(const BaseBook &bb);
      void del(const BaseBook &bb);
      void del(const std::string &bid);
      int show() const;
      std::string show(const int &num) const;
      void show(const std::string &bid) const;
      void schBook(const std::string &s) const;
      void sortByInTime(bool asc = true);
       //友元运算符重载声明
      friend std::istream &operator >>(std::istream &in, BookList &booklist);
      friend std::ostream &operator <<(std::ostream &out, const BookList &booklist);
 };
}

using namespace LibraryBookSystem;
#endif //BOOK_H