#include "aoc.h"
#include "bmp.h"

const char* main_delimeters = " ,";


void solve(bool first)
{
    ll res = 0;
    auto img = loadImage(ls); // rng::views::all(ls));
    cPosition pos;
    unordered_map<cPosition, vector<int>> gears; // for part two
    loop_row(pos, img)
    {
        loop_col(pos, img)
        {
            bool good = false;
            int v = 0;
            vector<cPosition> start_ps;
            while (img.isValidPos(pos) && img.at(pos) >= '0' && img.at(pos) <= '9')
            {
                img.forOffsets(pos, neighbour8Positions,
                    [&](cPosition p)
                    {
                        if (first)
                        {
                            if (auto ch = img.at(p); ch != '.' && (ch < '0' || ch > '9'))
                                good = true;
                        }
                        else
                        {
                            if (img.at(p) == '*')
                            {
                                start_ps.emplace_back(p);
                                good = true;
                            }
                        }
                     });
                v = v * 10 + img.at(pos) - '0';
                ++pos.col;
            }
            if (good)
            {
                if(first)
                    res += v;
                else
                {
                    rng::sort(start_ps);
                    start_ps.erase(unique(ALL(start_ps)), start_ps.end());
                    for (auto& p : start_ps)
                        gears[p].emplace_back(v);
                }
            }
        }
    }
    if (!first)
    {
        for (auto& [pos, values] : gears)
        {
            if (values.size() == 2)
            {
                res += values[0] * values[1];
            }
        }
    }
    P("%lld\n", res);
}


