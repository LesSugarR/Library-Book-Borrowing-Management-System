#include "Functions.h"
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

}
string getTime();
