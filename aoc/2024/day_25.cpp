#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,:";
bool main_allow_empty_fields = false;


void solve(bool first)
{
    ll res = 0;
    vector<cImage<char>> imgs;
    for (auto& b : blocks)
    {
        auto img = loadImage(b);
        for (auto& i : imgs)
            if (rng::all_of(img.allPos(), [&](auto p) { return i[p] == '.' || img[p] == '.'; }))
                ++res;
        imgs.emplace_back(img);
    }
    RESULT(res);
}
