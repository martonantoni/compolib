#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

void solve(bool first)
{
    int min_steps = first ? 0 : 4;
    int max_steps = first ? 3 : 10;
    cImage<char> img = loadImage(ls);
    img -= '0';
    vector<cImage<char>> visited((max_steps + 1) * 4);
    for(auto& v: visited)
    {
        v.initSize(img);
        fill(ALL(v.cells), '.');
    }
    min_heap<tuple<ll, cPosition, cPosition, int>> f;  // cost, pos, dir, steps
    f.emplace(0, cPosition{ 0,0 }, direction_NE, min_steps);
    while (!f.empty())
    {
        auto [cost, pos, dir, steps] = f.top();
        f.pop();
        if (pos == img.bottomRight() && steps >= min_steps)
        {
            P("best: %lld\n", cost);
            return;
        }
        int diri = 0;
        for (auto& next_dir : neighbour4Positions)
        {
            if (next_dir == dir || steps >= min_steps)
            {
                if (next_dir != -dir && (next_dir != dir || steps < max_steps))
                {
                    cPosition next_pos = pos + next_dir;
                    int next_steps = next_dir == dir ? steps + 1 : 1;
                    int vsi = next_steps * 4 + diri;
                    if (img.isValidPos(next_pos) && visited[vsi][next_pos] == '.')
                    {
                        visited[vsi][next_pos] = '#';
                        f.emplace(cost + img[next_pos], next_pos, next_dir, next_steps);
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
