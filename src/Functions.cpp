#include "../include/Functions.h"
#define endl '\n'
#include<bits/stdc++.h>
using namespace std;
bool PositiveInt(const string &s)//检验是否是数字
{
    int len = s.length();
    for(int i = 0; i < len; i++)
    {
        if(s[i] >= '0' && s[i] <= '9')
            return true;
        else
            return false;
    }

}
void replace(string &s,const string &a,const string &b)
{
   auto x=s.find(a);
    while(x!=string::npos){
        s.replace(x, a.length(), b); // 替换整个 a 的长度
        x = s.find(a, x + b.length());
    }
}
string getTime()
{
    time_t t;
    time(&t);
    string s = ctime(&t);
    if (s.empty() && s.back()== '\n')s.pop_back();
    for (int i = 0; i < s.size(); i++)
        if (s[i] == ' ') s[i] = '_';
    return s;
}
