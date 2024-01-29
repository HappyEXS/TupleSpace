#ifndef TUPLE_H
#define TUPLE_H

#include <bits/stdc++.h>
#include <ostream>
#include <variant>

using namespace std;

class Tuple {
public:
    string tupleStr;
    bool isValid;
    vector<variant<string, int, float>> elements;
    explicit Tuple(const string& str);
    string toString();
private:
    vector<variant<string, int, float>> parseFromString(string str);
    static vector<string> split(const string& s, const string& delimiter);
};

#endif