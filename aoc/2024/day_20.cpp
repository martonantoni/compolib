#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

cPosition start_pos, end_pos;

cImage<int> best_from_start;
cImage<int> best_from_end;

void fill_best(cImage<char>& img, cPosition start, cImage<int>& best)
{
    best.w = img.w;
    best.h = img.h;
    best.cells.clear();
    best.cells.resize(best.w * best.h, numeric_limits<int>::max());
    best[start] = 0;
    vector<pair<cPosition, int>> f; // pos, cost
    f.emplace_back(start, 0);
    while (!f.empty())
    {
        auto [pos, c] = f.front();
        pop_heap(f.begin(), f.end(), [](auto& l, auto& r) { return l.second > r.second; });
        f.pop_back();
        for (auto n : neighbour4Positions)
        {
            auto np = pos + n;
            if (!img.isValidPos(np))
            {
                continue;
            }
            if (img[np] == '#')
            {
                continue;
            }
            if (best[np] <= c + 1)
            {
                continue;
            }
            best[np] = c + 1;
            f.emplace_back(np, c + 1);
            push_heap(f.begin(), f.end(), [](auto& l, auto& r) { return l.second > r.second; });
        }
    }
}

void solve(bool first)
{
    if (is_example)
        return;
    ll res = 0;

    auto img = loadImage(ls);

    for (auto pos : img.allPos())
    {
        if (img[pos] == 'S')
        {
            start_pos = pos;
            img[pos] = '.';
        }
        else if (img[pos] == 'E')
        {
            end_pos = pos;
            img[pos] = '.';
        }
    }
    fill_best(img, start_pos, best_from_start);
    fill_best(img, end_pos, best_from_end);

    auto base = best_from_start[end_pos];

    vector<cPosition> offsets;
    int max_dist = first ? 2 : 20;
    for (int x = -max_dist; x <= max_dist; ++x)
    {
        for (int y = -max_dist; y <= max_dist; ++y)
        {
            int ny_dist = abs(x) + abs(y);
            if (ny_dist < 2 || ny_dist > max_dist)
            {
                continue;
            }
            offsets.emplace_back(x, y);
        }
    }

    for (auto c_start : img.allPos())
    {
        if (img[c_start] != '.')
        {
            continue;
        }
        for (auto offset : offsets)
        {
            auto c_end = c_start + offset;
            if (!img.isValidPos(c_end) || img[c_end] != '.')
            {
                continue;
            }
            int ny_dist = abs(c_start.row - c_end.row) + abs(c_start.col - c_end.col);
            int dist = best_from_start[c_start] + best_from_end[c_end] + ny_dist;
            int save = base - dist;
            if (save >= 100)
            {
                ++res;
            }
        }
    }

    RESULT(res);
}


