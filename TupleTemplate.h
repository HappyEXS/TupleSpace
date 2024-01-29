#ifndef TUPLETEMPLATE_H
#define TUPLETEMPLATE_H

#include <bits/stdc++.h>
#include <ostream>

using namespace std;

class TupleTemplate {
public:
    string tupleStr;
    bool isValid;
    vector<string> elements;
    explicit TupleTemplate(const string& str);
    string toString();
private:
    static vector<string> split(const string& s, const string& delimiter);
    bool checkValid();
};

#endif