#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* main_delimeters = " ,()#";
bool main_allow_empty_fields = false;

void solveFirst()
{
    cImage<char> img;
    img.w = 1000;
    img.h = 1000;
    img.cells.resize(img.w * img.h, '.');
    cPosition pos = img.middle();
    img[pos] = '#';
    cPosition dm[256];
    dm['U'] = direction_N;
    dm['D'] = direction_S;
    dm['L'] = direction_W;
    dm['R'] = direction_E;
    for (auto& l : ls)
    {
        cPosition d = dm[l.s[0][0]];
        for(int i: vs::iota(0, l.i[1]))
        {
            pos += d;
            img[pos] = '#';
        }
    }
    floodFillImage(img, { 0,0 }, 'x', '#');

    P("result: %lld\n", count(ALL(img.cells), '#') + count(ALL(img.cells), '.'));
}

/*
Each hexadecimal code is six hexadecimal digits long. The first five hexadecimal digits encode the distance in meters
as a five-digit hexadecimal number. The last hexadecimal digit encodes the direction to dig: 
0 means R, 1 means D, 2 means L, and 3 means U.
*/

void solveSecond()
{
    vector<cPosition> cs;
    cPosition position { 0, 0 };
    array<cPosition, 4> dirs = { direction_E, direction_S, direction_W, direction_N };
    ll lineArea = 0;
    for (auto& l : ls)
    {
        cPosition dir = dirs[l.s[2][5] - '0'];
        ll len = stol(l.s[2].substr(0, 5), nullptr, 16); ;
        position += dir * len;
        lineArea += len;
        cs.emplace_back(position);
    }
    P("result: %lld\n", polygonArea(cs) + lineArea / 2ll + 1ll);
}

 void solve(bool first) { first ? solveFirst() : solveSecond(); }
