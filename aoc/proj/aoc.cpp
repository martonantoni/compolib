

#include "aoc.h"
#include "bmp.h"
#include "utils.h"

#include <bitset>

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;


void solve(bool first)
{
    ll res = 0;


    for (auto& l : ls)
    {
        auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;
    }

//    cImage<char> img;
//    P("res: {}", res);

    if (first) res = 1234565;

    RESULT(res);
}