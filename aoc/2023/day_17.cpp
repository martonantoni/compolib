#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

struct cPos
{
    cPosition p;
    cPosition d;
    int steps;
    ll cost;

    cPos(cPosition p, cPosition d, int steps, int cost) :
        p(p), d(d), steps(steps), cost(cost) {}

    bool operator<(const cPos& other) const {
        return cost > other.cost;
    }
};

void solve(bool first)
{
    ll res = 0;

    int min_steps = first ? 0 : 4;
    int max_steps = first? 3 : 10;

    cImage<char> img = loadImage(ls);
    vector<cImage<char>> visited(min_steps*max_steps + 20);
    for(auto& v: visited)
    {
        v.initSize(img);
        fill(ALL(v.cells), '.');
    }
    for (auto& c : img.cells)
        c -= '0';

    vector<cPos> f;
    f.emplace_back(cPosition{ 0,0 }, cPosition{ 1,1 }, min_steps, 0);
    cPosition target_pos{ img.h - 1, img.w - 1 };

    while (!f.empty())
    {
        auto p = f.front();
        pop_heap(ALL(f), [](auto& l, auto& r) { return l < r; });
        f.pop_back();
        if (p.p == target_pos && p.steps >= min_steps)
        {
            P("best: %lld\n", p.cost);
            return;
        }

        int diri = 0;
        for (auto& next_dir : neighbour4Positions)
        {
            if (next_dir == p.d || p.steps >= min_steps)
            {
                if (next_dir != -p.d && (next_dir != p.d || p.steps < max_steps))
                {
                    cPosition next_pos = p.p + next_dir;
                    int next_steps = next_dir == p.d ? p.steps + 1 : 1;
                    int vsi = next_steps * 4 + diri;
                    if (img.isValidPos(next_pos) && visited[vsi][next_pos] == '.')
                    {
                        visited[vsi][next_pos] = '#';
                        cPos np(next_pos, next_dir, next_steps, p.cost + img[next_pos]);
                        f.emplace_back(np);
                        push_heap(ALL(f), [](auto& l, auto& r) { return l < r; });
                    }
                }
            }
            ++diri;
        }
    }
}

// void solveSecond()
// {
// }

// void solve(bool first) { first ? solveFirst() : solveSecond(); }
