#include "aoc.h"
#include "bmp.h"

const char* main_delimeters = " ,";


void solve(bool first)
{
    ll res = first ? 0 : ls.size();
    vector<ll> card_counts(ls.size(), 1);
    for (auto& l : ls)
    {
        cStringVector ps(l.txt, ":|");
        cIntVector ws = cStringVector(ps[1], " ", false).ToIntVector();
        cIntVector ns = cStringVector(ps[2], " ", false).ToIntVector();

        ll hits = 0;
        for (ll w : ws)
            hits += count(ALL(ns), w);

        if (hits > 0)
        {
            if (first)
                res += 1ll << (hits - 1);
            else
            {
                for (auto& x : card_counts | views::drop(l.idx + 1) | views::take(hits))
                    x += card_counts[l.idx];
            }
        }
    }
    if (!first)
        res = accumulate(ALL(card_counts), 0ll);
    P("%lld\n", res);
}


