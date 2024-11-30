#include "bmp.h"

ll polygonArea(const vector<cPosition>& points)
{
    ll a = 0;
    FOR(i, points.size())
    {
        auto& p = points[i];
        auto& pn = points[(i + 1) % points.size()];
        a += p.row * pn.col - pn.row * p.col;
    }
    return abs(a / 2);
}

void printImage(cImage<char>& img)
{
    cPosition pos;
    pos.col = 0;
    loop_row(pos, img)
    {
        P("%.*s", img.w, &img[pos]);
    }
    P("\n");
}

void floodFillImage(cImage<char>& img, cPosition startPos, char fillChar, char borderChar)
{
    vector<cPosition> f;
    f.emplace_back(0, 0);
    while (!f.empty())
    {
        auto p = f.back();
        f.pop_back();
        if (img[p] == fillChar || img[p] == borderChar)
            continue;
        img[p] = fillChar;
        img.forEach4Neighbour(p, [&](cPosition np)
            {
                f.emplace_back(np);
            });
    }

}
