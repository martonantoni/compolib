#include "aoc.h"
#include "bmp.h"

const char* main_delimeters = " ,";

void solveFirst()
{
    ll res = 0;
    ls[0].i.erase(ls[0].i.begin());
    auto& seeds = ls[0].i;
    vector<bool> done(seeds.size(), false);

    for (int i = 1; i < ls.size(); ++i)
    {
        auto& line = ls[i];
        if (line.txt.empty())
        {
            fill(ALL(done), false);
            ++i;
            continue;
        }
        ll dest = line.i[0], src = line.i[1], r = line.i[2];
        for (int j = 0; j < seeds.size(); ++j)
        {
            if (!done[j])
            {
                ll& seed = seeds[j];
                if (seed >= src && seed < src + r)
                {
                    seed = dest + (seed - src);
                    done[j] = true;
                }
            }
        }
    }
    P("----> Result: %lld\n", *min_element(ALL(seeds)));
}

void solveSecond()
{
    ll res = 0;
    ls[0].i.erase(ls[0].i.begin());
    auto& seeds = ls[0].i;
    vector<pair<ll, ll>> ranges; // from, to
    for (int i = 0; i < ls[0].i.size(); i += 2)
    {
        ranges.emplace_back(ls[0].i[i], ls[0].i[i] + ls[0].i[i + 1] - 1);
    }
    vector<pair<ll, ll>> new_ranges;// = ranges;

    for (int i = 1; i < ls.size(); ++i)
    {
        auto& line = ls[i];
        if (line.txt.empty())
        {
            ++i;
            ranges.insert(ranges.end(), ALL(new_ranges));
            new_ranges.clear();
            continue;
        }
        ll dest = line.i[0], src = line.i[1], r = line.i[2];
        ll src_to = src + r - 1;

        vector<pair<ll, ll>> remaining_ranges;
        for (auto& r : ranges)
        {
            if (src_to<r.first || src > r.second)
            {
                remaining_ranges.emplace_back(r);
                continue;
            }
            ll before_length = src - r.first;
            if (before_length > 0)
            {
                remaining_ranges.emplace_back(r.first, src - 1);
                r.first = src;
            }
            ll after_length = r.second - src_to;
            if (after_length > 0)
            {
                remaining_ranges.emplace_back(src_to + 1, r.second);
                r.second = src_to;
            }
            ll range_length = r.second - r.first + 1;
            new_ranges.emplace_back(dest + r.first - src, dest + r.first - src + range_length - 1);
        }
        ranges = move(remaining_ranges);
    }
    for (auto& r : new_ranges)
        ranges.emplace_back(r);
    P("----> Result: %lld\n", min_element(ALL(ranges))->first);
}

void solve(bool first) { first ? solveFirst() : solveSecond(); }
