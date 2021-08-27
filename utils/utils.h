
#ifndef __UTILS__
#define __UTILS__
#include <string>
#include <vector>
using std::string;
using std::vector;

#include <iostream>
#include <iterator>
#include <regex>


void s_split(const string &s, vector<string> &tokens, char delim = ' ')
{
    tokens.clear();
    auto string_find_first_not = [s, delim](size_t pos = 0) -> size_t {
        for (size_t i = pos; i < s.size(); i++) {
            if (s[i] != delim)
                return i;
        }
        return string::npos;
    };
    size_t lastPos = string_find_first_not(0);
    size_t pos = s.find(delim, lastPos);
    while (lastPos != string::npos) {
        tokens.emplace_back(s.substr(lastPos, pos - lastPos));
        lastPos = string_find_first_not(pos);
        pos = s.find(delim, lastPos);
    }
}

static void getNumberByReg(std::string &str, std::vector<int> &res)
{
    std::regex e(R"(\d+)");
    std::sregex_iterator iter(str.begin(), str.end(), e);
    std::sregex_iterator end;

    while (iter != end) {
        for (int i = 0; i < iter->size(); ++i) {
            res.push_back(std::stoi((*iter)[i]));
        }
        ++iter;
    }
}

#endif