#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include<bits/stdc++.h>
#include "Book.h"
using namespace std;
class Library;
struct Date
{
	int year, month, day;
	bool operator<(const Date& x) const;
    string toString();
};
enum OpType
{
	ADD,
	DELETE,
	MODIFY
};
struct UndoRecord
{
	OpType op;
	Book date;
};
bool PositiveInt(const string &s);

string getTime();
//随机生成工具
int randomInt(int min, int max);
string randomString(int len);
char randomChar();
string randomId();
string randomName();
string randomAuthor();
string randomPress();
int randomTotalNum();
int randomBorrowNum();
string randomInTime();
//通用字符串工具
string trim(const string &s);
void replace(string &s,const string &a,const string &b);
string joinStrings(string s[]);
//Hash/编码工具
int hashFunction(string &s);
string normalizeString(string &s);
string encodeKeyword(string &s);
//序列化/反序列化工具
string encodeBook(BaseBook* book);
BaseBook* decodeBook(const string& line);
string encodeHistory(const BaseHistory& bh);
BaseHistory decodeBaseHistory(const string& line);
StringArray encodeHistoryList(const HistoryList& hl);
HistoryList* decodeHistoryList(const StringArray& arr);
BookList* decodeList(const vector<string>& lines);
//文件OI
bool saveBooksToFile(const string& path, BookList* head);
BookList* loadBooksFromFile(const string& path);
bool saveHistoryToFile(const string& path, HistoryList* head);
HistoryList* loadHistoryFromFile(const string& path);
bool appendLog(const string& path, const string& message);
bool clearFile(const string& path);
bool fileExists(const string& path);
//数据结构工具
void printList(BookList* head);
void printHistory(HistoryList* head);
BookList* deepCopyList(BookList* head);
HistoryList* deepCopyHistory(HistoryList* head);
//性能工具
void timerStart();
long long timerStop();
int countMemoryOfList(BookList* head);
int countMemoryOfHistory(HistoryList* head);
//web API工具
string jsonEscape(const string& s);
string buildJsonResponse(int status, const string& message, const string& data);
//系统工具
string generateUUID();
bool validateISBN(const string& s);
string safeInputString();
#endif //FUNCTIONS_H
