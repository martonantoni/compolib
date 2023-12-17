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
    cImage<unordered_map<int, bool>> visited(img);
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
            cPosition next_pos = pos + next_dir;
            if (img.isValidPos(next_pos) && (next_dir == dir || steps >= min_steps) && next_dir != -dir && (next_dir != dir || steps < max_steps))
            {                
                int next_steps = next_dir == dir ? steps + 1 : 1;
                if (auto& v = visited[next_pos][next_steps * 4 + diri]; !v)
                {
                    v = true;
                    f.emplace(cost + img[next_pos], next_pos, next_dir, next_steps);
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
