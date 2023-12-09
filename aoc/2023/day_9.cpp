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
            if(first)
                vals[i - 1].emplace_back(vals[i - 1].back() + vals[i].back());
            else
                vals[i - 1].insert(vals[i - 1].begin(), vals[i - 1][0] - vals[i].front());
        }
        res += first ? vals[0].back() : vals[0].front();
    }

    P("%lld\n", res);
}

// void solveSecond()
// {
// }

// void solve(bool first) { first ? solveFirst() : solveSecond(); }
