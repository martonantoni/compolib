#include "aoc.h"
#include "bmp.h"

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

void solve(bool first)
{
    ll res = 0;
//     for (auto& l : ls)
//     {
// 
//     }

    cImage<char> img = loadImage(ls);
    vector<cPosition> gs;
    cPosition p;
    loop_row(p, img)
    {
        loop_col(p, img)
        {
            if (img[p] == '#')
                gs.emplace_back(p);
        }
    }
    vector<ll> gaps;
    loop_row(p, img)
    {
        if (none_of(ALL(gs), [&](cPosition pos) { return  p.row == pos.row; }))
        {
            gaps.emplace_back(p.row);
        }
    }

    constexpr ll mul = 1000000ll - 1ll;

    for (auto& pos : gs)
    {
        ll inc = 0;
        for (auto& g : gaps)
        {
            if (g < pos.row)
                ++inc;
        }
        pos.row += inc * mul;
    }
    gaps.clear();


    loop_col(p, img)
    {
        if (none_of(ALL(gs), [&](cPosition pos) { return  p.col == pos.col; }))
        {
            gaps.emplace_back(p.col);
        }
    }
    for (auto& pos : gs)
    {
        ll inc = 0;
        for (auto& g : gaps)
        {
            if (g < pos.col)
                ++inc;
        }
        pos.col += inc * mul;
    }

    for (size_t i = 0; i < gs.size(); ++i)
    {
        for (size_t j = i + 1; j < gs.size(); ++j)
        {
            res += abs(gs[i].row - gs[j].row) + abs(gs[i].col - gs[j].col);
        }
    }

    P("%lld\n", res);
}


// void solveSecond()
// {
// }

// void solve(bool first) { first ? solveFirst() : solveSecond(); }
