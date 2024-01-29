#include "Tuple.h"

Tuple::Tuple(const string& str) {
    tupleStr = str;
    isValid = false;
    elements = parseFromString(str);
}

bool is_number(const string& s) {
    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

bool is_float(const string& s) {
    string::const_iterator it = s.begin();
    while (it != s.end() && (isdigit(*it) || *it == '.')) ++it;
    return !s.empty() && it == s.end();
}

vector<variant<string, int, float>> Tuple::parseFromString(string str) {
    vector<variant<string, int, float>> tuple;
    size_t length = str.length();

    if (str[0] == '(' && str[length-1] == ')') {
        str = str.substr(1, length - 2);
    } else {
        isValid = false;
        return tuple;
    }
    vector<string> elements_str = split(str, ",");
    for (auto elem: elements_str) {
        if (elem[0] == '"' && elem[elem.length()-1] == '"') {
            string element = elem.substr(1, elem.length() - 2);
            tuple.push_back(element);
        } else if (is_number(elem)) {
            int element = stoi(elem);
            tuple.push_back(element);
        } else if (is_float(elem)) {
            float element = stof(elem);
            tuple.push_back(element);
        } else {
            isValid = false;
            return tuple;
        }
    }
    isValid = true;
    return tuple;
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

vector<string> Tuple::split(const string& s, const string& delimiter) {
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

string Tuple::toString()  {
    string formattedTuple = "( ";
    for (auto element : elements) {
        if (auto *str = get_if<string>(&element)) {
            formattedTuple += ('"' + *str + '"' + ", ");
        } else if (auto *integer = get_if<int>(&element)) {
            formattedTuple += (to_string(*integer) + ", " );
        } else if (auto *floating = get_if<float>(&element)) {
            formattedTuple += (to_string(*floating) + ", ");
        }
    }
    return formattedTuple.substr(0, formattedTuple.length()-2) += " )";
}