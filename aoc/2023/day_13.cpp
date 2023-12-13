#include "aoc.h"
#include "bmp.h"

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

void print_vll(vector<ll>& nums)
{
    for (auto x : nums)
        P("%lld ", x);
}

bool tryReflectHoriz(cImage<char>& img, vector<cPosition>& positions, int afterRow)
{
    for (auto& p : positions)
    {
        cPosition rp(afterRow + (afterRow - p.row + 1), p.col);
        if (img.isValidPos(rp) && img[rp] == '.')
            return false;
    }
    return true;
}

bool tryReflectVert(cImage<char>& img, vector<cPosition>& ps, int afterCol)
{
    for (auto& p : ps)
    {
        cPosition rp(p.row,  afterCol + (afterCol - p.col + 1));
        if (img.isValidPos(rp) && img[rp] == '.')
            return false;
    }
    return true;
}

ll process(cImage<char>& img, ll orig)
{
    vector<cPosition> ps;
    img.forAll([&](cPosition pos) { if (img[pos] == '#') ps.emplace_back(pos); });
    for(int i = 0; i < img.h - 1;++i)
    {
        if (orig != 100 * (i + 1) && tryReflectHoriz(img, ps, i))
        {
            ++i;
//             P("found horiz %d\n", i);
            return 100 * i;
        }
    }
    for (int i = 0; i < img.w - 1; ++i)
    {
        if (orig != i+1 && tryReflectVert(img, ps, i))
        {
            ++i;
//             P("found vert %d\n", i);
            return i;
        }
    }
    return -1ll;
}

ll process2(cImage<char> img)
{
    ll orig = process(img, -1ll);
    cPosition pos;
//     P("img w: %d, h: %d\n", img.w, img.h);
    loop_row(pos, img)
    {
        loop_col(pos, img)
        {
//              P("%d, %d\n", pos.row, pos.col);
            img[pos] = img[pos] == '.' ? '#' : '.';
            ll d = process(img, orig);
            if (d != -1ll && d != orig)
            {
                P("found, d: %lld\n", d);
                return d;
            }
            img[pos] = img[pos] == '.' ? '#' : '.';
        }
    }
    P("-------------------------NOT FOUND\n");
    return orig;
}


void solve(bool first)
{
    ll res = 0;
    ll start_row = 0;
    ls.emplace_back().is_empty = true;
    ls.back().idx = ls.size() - 1;
    for (auto& l : ls)
    {
        if (l.is_empty)
        {
            auto img = loadImage(ls | views::drop(start_row) | views::take(l.idx - start_row));
            res += process2(img);
            start_row = l.idx + 1;
        }
    }

    P("%lld\n", res);
}


// void solveSecond()
// {
// }

// void solve(bool first) { first ? solveFirst() : solveSecond(); }
