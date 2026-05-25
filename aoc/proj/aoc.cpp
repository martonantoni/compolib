

#include "aoc.h"
#include "bmp.h"
#include "utils.h"

#include <bitset>

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ";
bool main_allow_empty_fields = false;


void solve(bool first)
{
    ll res = 0;

    vector<pair<int, string>> rs;

    for (auto& l : ls)
    {
        auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;

        auto& r = rs.emplace_back();
        r.first = is.back();
        for (int i = 2; i < is.size() - 1; ++i)
        {
            r.second += format("{} ", s[i]);
        }
        r.second.pop_back();

    }
    rng::sort(rs, greater<>());
    for(auto& [v, s]: rs)
    {
        P("{}: {}", v, s);
    }

//    cImage<char> img;
//    P("res: {}", res);

    if (first) res = 1234565;

    RESULT(res);
}