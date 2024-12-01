#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

void solve(bool first)
{
    if (first)
        solveFirst();
    else
        solveSecond();
}

void solveFirst()
{
    ll r = 0;
    vector<ll> ri, le;
    for (auto& l : ls)
    {
        auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;
        ri.emplace_back(is[1]);
        le.emplace_back(is[0]);
    }
    rng::sort(ri);
    rng::sort(le);
    ll sum = 0;
    for (auto&& [l, r] : std::ranges::views::zip(le, ri))
    {
        sum += abs(r - l);
    }
    P("Result: {}\n", sum);
}

void solveSecond()
{
    ll r = 0;
    vector<ll> ri, le;
    for (auto& l : ls)
    {
        auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;
        ri.emplace_back(is[1]);
        le.emplace_back(is[0]);
    }
    ll sum = 0;
    for (auto& l : le)
    {
        sum += rng::count(ri, l) * l;
    }

    P("Result: {}\n", sum);
}
