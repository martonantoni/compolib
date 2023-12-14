#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

void print_vll(vector<ll>& nums)
{
    for (auto x : nums)
        P("%lld ", x);
}

constexpr ll cn = 1'000'000'000;

void roll_n(cImage<char>& img)
{
    cPosition pos;
    loop_col(pos, img)
    {
        int target_row = 0;
        loop_row(pos, img)
        {
            if (img[pos] == '#')
            {
                target_row = pos.row + 1;
            }
            if (img[pos] == 'O')
            {
                img[pos] = '.';
                img[cPosition{ target_row, pos.col }] = 'O';
                ++target_row;
            }
        }
    }
}

void roll_s(cImage<char>& img)
{
    cPosition pos;
    loop_col(pos, img)
    {
        int target_row = img.h-1;
        for(pos.row = img.h-1; pos.row>=0; --pos.row)
        {
            if (img[pos] == '#')
            {
                target_row = pos.row - 1;
            }
            if (img[pos] == 'O')
            {
                img[pos] = '.';
                img[cPosition{ target_row, pos.col }] = 'O';
                --target_row;
            }
        }
    }
}

void roll_w(cImage<char>& img)
{
    cPosition pos;
    loop_row(pos, img)
    {
        int target_col = 0;
        loop_col(pos, img)
        {
            if (img[pos] == '#')
            {
                target_col = pos.col + 1;
            }
            if (img[pos] == 'O')
            {
                img[pos] = '.';
                img[cPosition{ pos.row , target_col }] = 'O';
                ++target_col;
            }
        }
    }
}

void roll_e(cImage<char>& img)
{
    cPosition pos;
    loop_row(pos, img)
    {
        int target_col = img.w - 1;
        for (pos.col = img.w - 1; pos.col >= 0; --pos.col)
        {
            if (img[pos] == '#')
            {
                target_col = pos.col - 1;
            }
            if (img[pos] == 'O')
            {
                img[pos] = '.';
                img[cPosition{ pos.row , target_col }] = 'O';
                --target_col;
            }
        }
    }
}

ll calc2(cImage<char>& img)
{
    ll res = 0;
    cPosition pos;
    loop_col(pos, img)
    {
        loop_row(pos, img)
        {
            if (img[pos] == 'O')
            {
                res += img.h - pos.row;
            }
        }
    }
    return res;
}

void solve(bool first)
{
    ll res = 0;
    auto img = loadImage(ls);
    if (first)
    {
        roll_n(img);
        P("result: %lld\n", calc2(img));
        return;
    }

    vector<ll> results;
    FOR(i, 1000)
    {
        roll_n(img);
        roll_w(img);
        roll_s(img);
        roll_e(img);
        results.emplace_back(calc2(img));
    }
    P("result: %lld\n", cPattern(results)[cn - 1ll]);
}


// void solveSecond()
// {
// }

// void solve(bool first) { first ? solveFirst() : solveSecond(); }
