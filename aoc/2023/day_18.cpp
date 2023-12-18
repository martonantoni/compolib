#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* main_delimeters = " ,()#";
bool main_allow_empty_fields = false;

void solveFirst()
{
    ll res = 0;

    cImage<char> img;
    img.w = 1000;
    img.h = 1000;
    img.cells.resize(img.w * img.h, '.');

    cPosition pos(img.h / 2, img.w / 2);
    img[pos] = '#';

    // up (U), down (D), left (L), or right (R), 

    for (auto& l : ls)
    {
        cPosition d;
        switch (l.s[0][0])
        {
        case 'U':
            d = direction_N;
            break;
        case 'D':
            d = direction_S;
            break;
        case 'L':
            d = direction_W;
            break;
        case 'R':
            d = direction_E;
            break;
        }
        for (int i = 0; i < l.i[1]; ++i)
        {
            pos += d;
            img[pos] = '#';
        }
    }


    vector<cPosition> f;
    f.emplace_back(0, 0);

    while (!f.empty())
    {
        auto p = f.back();
        f.pop_back();
        if (img[p] == '#' || img[p]=='x')
            continue;
        img[p] = 'x';
        img.forEach4Neighbour(p, [&](cPosition np)
            {
                if(img[np]=='.')
                    f.emplace_back(np);
            });
    }
    P("result: %lld\n", count(ALL(img.cells), '#') + count(ALL(img.cells), '.'));
}

/*
Each hexadecimal code is six hexadecimal digits long. The first five hexadecimal digits encode the distance in meters
as a five-digit hexadecimal number. The last hexadecimal digit encodes the direction to dig: 
0 means R, 1 means D, 2 means L, and 3 means U.
*/

ll calcArea(const vector<cPosition>& ps) 
{
    ll a = 0;
    FOR(i, ps.size())
    {
        auto& p = ps[i];
        auto& pn = ps[(i + 1) % ps.size()];
        a += p.row * pn.col - pn.row * p.col;
    }
    return abs(a/2);
}

void solveSecond()
{
    vector<cPosition> cs;
    cPosition p{ 0, 0 };
    array<cPosition, 4> dirs = { direction_E, direction_S, direction_W, direction_N };

    ll min_h = 0, max_h = 0;
    ll laer = 0;
    for (auto& l : ls)
    {
        cPosition d = dirs[l.s[2][5] - '0'];
        ll len = stol(l.s[2].substr(0, 5), nullptr, 16); ;
        P("%lld\n", len);
        p += d * len;
        laer += len;
        cs.emplace_back(p);
        min_h = min(p.row, min_h);
        max_h = max(p.row, max_h);
    }

    P("min: %lld, max: %lld\n", min_h, max_h);
    ll a = calcArea(cs);
    P("a: %lld (%lld), laer: %lld, area: %lld (%lld)\n", a, 952408144115ll - a,  laer, a + laer, 952408144115ll - (a+laer));
    P("guess: %lld\n", a + (laer / 2ll) + 1ll);
}

 void solve(bool first) { first ? solveFirst() : solveSecond(); }
