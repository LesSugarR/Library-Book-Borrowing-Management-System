#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <bits/stdc++.h>
using namespace std;

bool isPositiveInt(const string &s);
void replace(string &s,const string &a,const string &b);
string getTime();
static bool safe_getline(istream& in, string& s) {
    if (!std::getline(in, s)) return false;
    if (!s.empty() && s.back() == '\r') s.pop_back();
    return true;
}
#endif
