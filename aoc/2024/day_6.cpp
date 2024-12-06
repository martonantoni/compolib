#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

bool isInfinite(cImage<char> img, cPosition gp, cDirection dir, cPosition extra)
{
    img[extra] = '#';

    vector<cImage<char>> visited(8, { img, false });

    for(;;)
    {
        if (visited[dir][gp])
            return true;
        visited[dir][gp] = true;
        if (!img.isValidPos(gp + dir))
            return false;
        if (img[gp + dir] == '#')
            dir.rot90();
        else
            gp += dir;
    }
}

void solve(bool first)
{
    auto img = loadImage(ls);

    cPosition gp;
    for (auto p : img.allPos())
    {
        if (img[p] != '.' && img[p] != '#')
        {
            gp = p;
            break;
        }
    }
    cDirection dir = cDirection::fromMark(img[gp]);

    ll sum = 0;
    if (first)
    {
        while (img.isValidPos(gp))
        {
            img[gp] = 'X';
            if (!img.isValidPos(gp + dir))
                break;
            if (img[gp + dir] == '#')
                dir.rot90();
            else
                gp += dir;
        }
        sum = rng::count(img.cells, 'X');
    }
    else
    {
        sum = rng::count_if(img.allPos(), [&](auto p) { return p != gp && isInfinite(img, gp, dir, p); });
    }
    P("result: {}", sum);
}
