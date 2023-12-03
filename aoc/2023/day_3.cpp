#include "aoc.h"
#include "bmp.h"

const char* main_delimeters = " ,";


void solve(bool first)
{
    ll res = 0;
    auto img = loadImage(ls); 
    cPosition pos;
    unordered_map<cPosition, vector<int>> gears; // for part two
    loop_row(pos, img)
    {
        loop_col(pos, img)
        {
            bool good = false;
            int v = 0;
            unordered_set<cPosition> gear_ps;
            while (img.isValidPos(pos) && isdigit(img.at(pos)))
            {
                img.forOffsets(pos, neighbour8Positions,
                    [&](cPosition p)
                    {
                        if (first)
                        {
                            if (auto ch = img.at(p); ch != '.' && !isdigit(ch))
                                good = true;
                        }
                        else
                        {
                            if (img.at(p) == '*')
                            {
                                gear_ps.insert(p);
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
                    for (auto& p : gear_ps)
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


