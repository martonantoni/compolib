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
    for(auto& l: ls)
        r += l.i[0];

    P("Result: {}\n", r);
}

void solveSecond()
{
    ll r = 0;
    unordered_set<ll> seen;
    for (;;)
    {
        for (auto& l : ls)
        {
            r += l.i[0];
            if (seen.contains(r))
            {
                P("Result: {}\n", r);
                return;
            }
            seen.insert(r);
        }
    }
}
