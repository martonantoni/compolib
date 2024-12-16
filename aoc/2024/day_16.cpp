#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

struct cState
{
    ll score;
    cPosition p;
    cDirection d;
};

void solve(bool first)
{
    ll res = 0;
    auto img = loadImage(blocks[0]);
    cImage<array<ll,4>> best;
    best.w = img.w;
    best.h = img.h;
    best.cells.resize(best.w * best.h);
    for (size_t i = 0; i < best.w*best.h; ++i)
    {
        for (int j = 0; j < 4; ++j)
            best.cells[i][j] = std::numeric_limits<ll>::max();
    }
    cPosition start_pos, end_pos;
    for (auto [p, c] : img.allPosV())
    {
        if (c == 'S')
            start_pos = p;
        else if (c == 'E')
            end_pos = p;
    }

    vector<cState> f;
    f.emplace_back(0, start_pos, cDirection::east());
    f.emplace_back(1000, start_pos, cDirection::north());
    f.emplace_back(1000, start_pos, cDirection::south());
    f.emplace_back(2000, start_pos, cDirection::west());
    best[start_pos][0] = 1000;
    best[start_pos][1] = 0;
    best[start_pos][2] = 1000;
    best[start_pos][3] = 2000;
    make_heap(ALL(f), [](const cState& l, const cState& r) { return l.score > r.score; });

    while (!f.empty())
    {
        auto [score, p, d] = f.front();
        pop_heap(ALL(f), [](const cState& l, const cState& r) { return l.score > r.score; });
        f.pop_back();
        if (p == end_pos)
        {
            if(res == 0)
                res = score;
            if(first)
                break;
        }
        cPosition target = p + d;
        if (img[target] == '#')
            continue;
        auto tryAdd = [&](ll score, cDirection new_dir)
            {
                if (score < best[target][new_dir.index / 2])
                {
                    best[target][new_dir.index / 2] = score;
                    f.emplace_back(score, target, new_dir);
                    push_heap(ALL(f), [](const cState& l, const cState& r) { return l.score > r.score; });
                }
            };
        tryAdd(score + 1, d);
        d.rot90(); // 90
        tryAdd(score + 1001, d);
        d.rot90(); // 180
        d.rot90(); // 270
        tryAdd(score + 1001, d); 
    }

    ll seats = 0;
    ++seats; // end_pos
    

    f.emplace_back(res - 1, end_pos, cDirection::east());
    f.emplace_back(res - 1, end_pos, cDirection::north());
    f.emplace_back(res - 1, end_pos, cDirection::west());
    f.emplace_back(res - 1, end_pos, cDirection::south());
    while (!f.empty())
    {
        auto [score, p, d] = f.back();
        f.pop_back();
        cDirection from_dir = d;
        from_dir.rot180();

        cPosition target = p + from_dir;

        if (best[target][d.index / 2] != score)
        {
       //     P("tried {}, {} with score {} but best is {}", target.row, target.col, score, best[target][d.index / 2]);
            continue;
        }
        if (img[target] == '.' || img[target]=='S')
        {
            ++seats;
            img[target] = 'V';
        }
        f.emplace_back(score - 1, target, d);
        d.rot90(); // 90
        f.emplace_back(score - 1001, target, d);
        d.rot90(); // 180
        d.rot90(); // 270
        f.emplace_back(score - 1001, target, d);
    }

    RESULT(first ? res : seats);
}