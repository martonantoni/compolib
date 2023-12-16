#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

ll calc(cImage<char>& img, cPosition start_pos, int start_dir)
{
    vector<pair<cPosition, int>> rays; // pos, dir
    rays.emplace_back(start_pos, start_dir);
    cImage<unsigned int> energy;
    energy.initSize(img);
    fill(ALL(energy.cells), 0);

    while (!rays.empty())
    {
        auto [ray_pos, ray_dir] = rays.back();
        rays.pop_back();
        if (energy[ray_pos] & (1 << ray_dir))
        {
            continue;
        }
        energy[ray_pos] |= 1 << ray_dir;

        auto step = [&rays, &img](cPosition pos, int dir)
        {
            pos += neighbour4Positions[dir];
            if (img.isValidPos(pos))
            {
                rays.emplace_back(pos, dir);
            }
        };

        switch (img[ray_pos])
        {
        case '.':
            step(ray_pos, ray_dir);
            break;
        case '|':
            if (ray_dir == 0 || ray_dir == 2)
            {
                step(ray_pos, ray_dir);
            }
            else
            {
                step(ray_pos, 0);
                step(ray_pos, 2);
            }
            break;
        case '-':
            if (ray_dir == 1 || ray_dir == 3)
            {
                step(ray_pos, ray_dir);
            }
            else
            {
                step(ray_pos, 3);
                step(ray_pos, 1);
            }
            break;
        case '/':
        {
            int nd[4] = { 1, 0, 3, 2 };
            step(ray_pos, nd[ray_dir]);
            break;
        }
        case '\\':
        {
            int nd[4] = { 3, 2, 1, 0 };
            step(ray_pos, nd[ray_dir]);
            break;
        }
        }
    }

    return count_if(ALL(energy.cells), [](auto e) { return e > 0; });

}

void solve(bool first)
{
    cImage<char> img = loadImage(ls);
    if (first)
    {
        P("result: %lld\n", calc(img, cPosition{ 0, 0 }, 1));
        return;
    }
    ll best = 0;
    FOR(row, img.h)
    {
        FOR(d, 4)
        {
            best = max(best, calc(img, cPosition{ row, 0 }, d));
            best = max(best, calc(img, cPosition{ row, img.w - 1 }, d));
        }
    }
    FOR(col, img.w)
    {
        FOR(d, 4)
        {
            best = max(best, calc(img, cPosition{ 0, col }, d));
            best = max(best, calc(img, cPosition{ img.h - 1, col }, d));
        }
    }
    P("best: %lld\n", best);
}

// void solveSecond()
// {
// }

// void solve(bool first) { first ? solveFirst() : solveSecond(); }
