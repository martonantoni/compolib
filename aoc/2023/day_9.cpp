#include "aoc.h"
#include "bmp.h"

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

void solve(bool first)
{
    ll res = 0;
    vector<vector<ll>> vals;
    for (auto& l : ls)
    {
        vals.clear();
        if (!first)
            rng::reverse(l.i);
        vals.emplace_back(l.i);
        for (;;)
        {
            auto& next = vals.emplace_back();
            auto& last = vals[vals.size() - 2];
            bool is_all_0 = true;
            for (size_t i = 1; i < last.size(); ++i)
            {
                is_all_0 = next.emplace_back(last[i] - last[i - 1]) == 0 && is_all_0;
            }
            if(is_all_0)
                break;
        }
        for (size_t i = vals.size() - 1; i > 0; --i)
        {
            vals[i - 1].emplace_back(vals[i - 1].back() + vals[i].back());
        }
        res += vals[0].back();
    }

    P("%lld\n", res);
}

// void solveSecond()
// {
// }

// void solve(bool first) { first ? solveFirst() : solveSecond(); }
