#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,|";
bool main_allow_empty_fields = false;

void solve(bool first)
{
    ll sum = 0;

    unordered_map<ll, vector<ll>> orders; 
    vector<vector<ll>> manuals;

    bool readingManual = false;
    for (auto& l : ls)
    {
        auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;
        if (l.is_empty)
        {
            readingManual = true;
            continue;
        }
        if (readingManual)
            manuals.emplace_back(l.i);
        else
            orders[l.i[0]].emplace_back(l.i[1]);
    }
    for (auto& manual : manuals)
    {
        auto sorted = manual;
        sort(ALL(sorted), [&](ll a, ll b) { return rng::contains(orders[a], b); });
        if ((first && sorted == manual) || (!first && sorted != manual))
            sum += sorted[sorted.size() / 2];
    }
    P("Result: {}\n", sum);
}




