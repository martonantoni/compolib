#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* main_delimeters = " ,~";
bool main_allow_empty_fields = false;

// 2,0,5~2,2,5

//using cCoord = tuple<ll, ll, ll>;
using cCoord = array<ll, 3>;

bool isBetween(ll c, ll l, ll r)
{
    return c >= l && c <= r;
}

struct cBrick
{
    cCoord c[2];
    int axis = 0;
    bool intersect(cBrick& other)
    {
        int ais = 0;
        for (int i = 0; i < 3; ++i)
        {
            if (isBetween(c[0][i], other.c[0][i], other.c[1][i]) ||
                isBetween(c[1][i], other.c[0][i], other.c[1][i]) ||
                isBetween(other.c[0][i], c[0][i], c[1][i]) ||
                isBetween(other.c[1][i], c[0][i], c[1][i]))
                ++ais;

        }
        return ais == 3;
    }
};

bool fall(int bi, vector<cBrick>& bricks)
{
    cBrick& b = bricks[bi];
    if (b.c[0][2] <= 1)
        return false;
    --b.c[0][2];
    --b.c[1][2];
    for (auto& ob : bricks)
    {
        if (&ob == &b)
            continue;
        if (b.intersect(ob))
        {
            ++b.c[0][2];
            ++b.c[1][2];
            return false;
        }
    }
    return true;
}

void solveFirst()
{
    ll res = 0;
    vector<cBrick> bs;
    for (auto& l : ls)
    {
        auto& b = bs.emplace_back();
        FOR(i, 6)
            b.c[i/3][i%3] = l.i[i];
        FOR(i, 3)
        {
            if (b.c[0][i] != b.c[1][i])
                b.axis = i;
            if (b.c[0][i] > b.c[1][i])
                swap(b.c[0][i], b.c[1][i]);
        }
    }
    int bss = (int)bs.size();
    for (;;)
    {
        ll fallen = 0;
        FOR(bi, bss)
        {
            if (fall(bi, bs))
                ++fallen;
        }
        if (!fallen)
            break;
    }
    vector<cBrick> old = bs;
    FOR(bi, bss)
    {
        printf("%d\n", bi);
        // remove bi:
        cBrick& b = bs[bi];
        b.c[0][2] = b.c[1][2] = -5;

        bool safe_to_remove = true;

        vector<int> fallen(bss, 0);

        for (;;)
        {
            bool fell = false;
            FOR(bj, bss)
            {
                if (bi == bj)
                    continue;
                if (fall(bj, bs))
                {
                    fell = true;
                    fallen[bj] = 1;
                }
            }
            if (!fell)
                break;
        }

        //if (safe_to_remove)
        //{
        //    P("safe to remove: #%d", bi);
        //    ++res;
        //}

        bs = old;

        res += count(ALL(fallen), 1);
    }
    P("result: %lld", res);;


//    cImage<char> img = loadImage(ls);
}


void solveSecond()
{
    ll res = 0;
    for (auto& l : ls)
    {
    }
    P("result: %lld", res);
}


 void solve(bool first) { first ? solveFirst() : solveSecond(); }

