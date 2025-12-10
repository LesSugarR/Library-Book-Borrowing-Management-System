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
void replace(string &s,const string &a,const string &b);
string getTime();
#endif //FUNCTIONS_H
