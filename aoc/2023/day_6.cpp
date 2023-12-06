#include "aoc.h"
#include "bmp.h"

const char* main_delimeters = " ,";

constexpr int rnum = 4;

array<ll, rnum> race_times{ 60 ,    80  ,   86    , 76 };
array<ll, rnum> records{ 601 ,  1163 ,  1559 ,  1300 };

void solveFirst()
{
    ll res = 1;
    FOR(r, rnum)
    {
        ll w = 0;
        FOR(i, race_times[r])
        {
            ll rem = race_times[r] - i;
            ll d = rem * i;
            if (d > records[r])
                ++w;
        }
        res*= w;
    }

    P("----> Result: %lld\n", res);
}

ll race_time{ 60808676 };
ll record{ 601116315591300 };

void solveSecond()
{
    ll w = 0;
    for(ll i=0;i< race_time;++i)
    {
        ll rem = race_time - i;
        ll d = rem * i;
        if (d > record)
            ++w;
    }
    P("----> Result: %lld\n", w);
}

void solve(bool first) { first ? solveFirst() : solveSecond(); }
