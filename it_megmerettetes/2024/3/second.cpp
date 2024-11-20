#include "aoc.h"
#include "bmp.h"
#include "utils.h"

/*
struct cLine
{
    vector<ll> i;
    cStringVector s;
    string txt;
    bool is_empty = false;
    int idx;
};
*/

const char* secondPartName = "osszeg";

namespace
{

}

void solveSecond()
{
    vector<ll>& nums = ls[0].i;

    vector<ll> sums;
    ll sum = 0;
    for (auto n : nums)
    {
        sum += n;
        sums.push_back(sum);
    }
    for (int i = 1; i <= 42; ++i)
    {
        for (int j = 0; j < sums.size() - i; ++j)
        {
            if (sums[j + i] - sums[j] == 42)
            {
                P("found: {} length\n", i);
                break;
            }
        }
    }

//    P("{}", rng::count_if(ls[0].i, [](ll x) { return x % 5 == 4 || x % 5 == 1; }));
}


