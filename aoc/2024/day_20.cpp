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

    P("base: {}", base);

    unordered_map<int, int> saves;

    for (auto c_start : img.allPos())
    {
        for (int c_length = 2; c_length <= (first ? 2 : 20); ++c_length)
        {
            for (auto c_end : img.allPos())
            {
                if (img[c_end] != '.' || img[c_start] != '.')
                {
                    continue;
                }
                int ny_dist = abs(c_start.row - c_end.row) + abs(c_start.col - c_end.col);
                if (ny_dist != c_length)
                {
                    continue;
                }
                int dist = best_from_start[c_start] + best_from_end[c_end] + ny_dist;
                int save = base - dist;
                if (save >= 100)
                {
                    ++res;
                }
                saves[save]++;
            }
        }
    }

    RESULT(res);
}


