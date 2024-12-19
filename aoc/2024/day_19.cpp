#include "aoc.h"
#include "bmp.h"
#include "utils.h"

#include <bitset>

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

vector<string> patterns;
unordered_map<string, optional<ll>> poss;

ll check(string txt)
{
    auto& p = poss[txt];
    if (p.has_value())
        return *p;
    p = 0;
    for (auto& pattern : patterns)
    {
        if (txt.size() < pattern.size())
            continue;
        if (txt.substr(0, pattern.size()) == pattern)
        {
            *p += check(txt.substr(pattern.size()));
        }
    }
    return *p;
}

void solve(bool first)
{
    poss.clear();

    ll res = 0;

    patterns = ls[0].s;
    poss[""] = 1;

    for (auto& l : blocks[1])
    {
        string& target = l.txt;
        ll ways = check(l.txt);
        if (first)
            res += ways != 0;
        else
            res += ways;
    }

    RESULT(res);
}


//for (auto& l : ls)
//{
//    auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;
//}
