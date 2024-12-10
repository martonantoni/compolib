#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

ll testTrail(cImage<char>& img, cPosition start, bool first)
{
    if (img[start] != '0')
        return 0;
    ll res = 0;
    cImage<char> visited(img, 0);
    vector<cPosition> v(1, start);
    while (!v.empty())
    {
        auto p = v.back();
        v.pop_back();
        if (first && visited[p] == 1)
            continue;
        visited[p] = 1;
        int height = img[p];
        if (height == '9')
            ++res;
        for (auto np : img.neighbours(p))
        {
            if (img[np] - height == 1)
                v.push_back(np);
        }
    }
    return res;
}

void solve(bool first)
{
    ll res = 0;
    auto img = loadImage(ls);
    for (auto p : img.allPos())
    {
        res += testTrail(img, p, first);
    }
    RESULT(res);
}