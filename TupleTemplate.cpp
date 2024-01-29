#include "TupleTemplate.h"

TupleTemplate::TupleTemplate(const string& str) {
    tupleStr = str;
    string templateStr;
    if (str[0] == '(' && str[str.length()-1] == ')')
    {
        templateStr = str.substr(1, str.length() - 2);
        elements = split(templateStr, ",");
        isValid = checkValid();
    }
    else {
        isValid = false;
    }
}

bool TupleTemplate::checkValid() {
    for (const string& s : elements) {
        if (s != "str" && s != "int" && s != "fl" )
            return false;
    }
    return true;
}

inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

vector<string> TupleTemplate::split(const string& s, const string& delimiter) {
    vector<string> result;
    string element;
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        element = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        ltrim(element);
        rtrim(element);
        result.push_back(element);
    }
    element = s.substr(pos_start);
    ltrim(element);
    rtrim(element);
    result.push_back(element);
    return result;
}

string TupleTemplate::toString()  {
    string formattedTuple = "( ";
    for (const string & element : elements) {
        formattedTuple += ( element + ", ");
    }
    return formattedTuple.substr(0, formattedTuple.length()-2) += " )";
}