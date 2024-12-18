#include "aoc.h"
#include "bmp.h"
#include "utils.h"

#include <bitset>

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

int findBest(cImage<char>& img, cPosition end_pos)
{
    cImage<int> best(img, (int)std::numeric_limits<int>::max());
    vector<pair<cPosition, int>> f;
    f.emplace_back(cPosition{ 0,0 }, 0);
    while (!f.empty())
    {
        auto [pos, cost] = f.front();
        pop_heap(ALL(f), [](auto& l, auto& r) { return l.second > r.second; });
        f.pop_back();
        if (pos == end_pos)
        {
            return cost;
        }
        for (auto np : img.neighbours(pos))
        {
            if (img[np] == '.')
            {
                if (best[np] <= cost + 1)
                    continue;
                best[np] = cost + 1;
                f.emplace_back(np, cost + 1);
                push_heap(ALL(f), [](auto& l, auto& r) { return l.second > r.second; });
            }
        }
    }
    return -1;
}

void solve(bool first)
{
    ll res = 0;

    int w = is_example ? 7 : 71;
    int h = w;

    vector<cPosition> falls;
    cPosition start_pos(0, 0);
    cPosition end_pos(w - 1, h - 1);

    for (auto& l : ls)
    {
        auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;
        falls.emplace_back(l.i[0], l.i[1]);
    }

    cImage<char> img;
    img.w = w;
    img.h = h;
    img.cells.resize(w * h, '.');


    int to = is_example ? 12 : 1024;

    for (int i = 0; i < to; ++i)
    {
        img[falls[i]] = '#';
    }
    if (first)
    {
        RESULT(findBest(img, end_pos));
    }
    else
    {
        auto res = value_lower_bound(to, falls.size(),
            [&](int to)
            {
                for (int i = 0; i < falls.size(); ++i)
                    img[falls[i]] = i < to ? '#' : '.';
                return findBest(img, end_pos) == -1 ? lower : higher;
            });

        P("res: {},{}", falls[res].row, falls[res].col);
    }
}