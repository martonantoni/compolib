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

bool isSafe(auto diffs)
{
    return rng::all_of(diffs, [](ll d) { return d > 0 && d < 4; })
        ||
        rng::all_of(diffs, [](ll d) { return d < 0 && d > -4; });
}

void solveFirst()
{
    ll r = 0;
    for (auto& l : ls)
    {
        auto diffs = vs::pairwise_transform(l.i, [](ll a, ll b) { return b - a; });
        if (isSafe(diffs))
            ++r;
    }
    ll sum = 0;
    P("Result: {}\n", r);
}

void solveSecond()
{
    ll r = 0;
    for (auto& l : ls)
    {
        auto& is = l.i;
        for (auto idx = 0; idx < is.size(); ++idx)
        {
            // leave out element at idx:
            auto is2 = is | vs::enumerate | 
                vs::filter([idx](auto&& p) { return get<0>(p) != idx; }) |
                vs::transform([](auto&& p) { return get<1>(p); }) ;
            auto diffs = vs::pairwise_transform(is2, [](ll a, ll b) { return b - a; });
            if (isSafe(diffs))
            {
                ++r;
                break;
            }
        }
    }

    P("Result: {}\n", r);
}
