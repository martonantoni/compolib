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
    ll sum = 0;
    for (auto& l : ls)
    {
        auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;

    }

    P("Result: {}\n", sum);
}

void solveSecond()
{
}
