#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;


void solve(bool first)
{
    if (first)
        solveFirst();
    else
        solveSecond();
}

bool isXMAS(cImage<char>& img, cPosition pos, cPosition dir)
{
    for (char c : "XMAS"s)
    {
        if (!img.isValidPos(pos))
            return false;
        if (img[pos] != c)
            return false;
        pos += dir;
    }
    return true;
}

bool isMAS(cImage<char>& img, cPosition center)
{
    if (img[center] != 'A')
        return false;
    for (auto dir : { direction_NE, direction_NW })
    {
        if (!img.isValidPos(center + dir) || !img.isValidPos(center - dir))
            return false;
        if (img[center + dir] + img[center - dir] != 'M' + 'S')
            return false;
    }
    return true;
}

void solveFirst()
{
    ll sum = 0;

    cImage img = loadImage(ls);
    for (auto p : img.allPos())
    {
        sum += rng::count_if(neighbour8Positions, [&](auto dir) { return isXMAS(img, p, dir); });
    }

    P("Result: {}\n", sum);
}

void solveSecond()
{
    cImage img = loadImage(ls);
    ll sum = rng::count_if(img.allPos(), [&](auto p) { return isMAS(img, p); });
    P("Result: {}\n", sum);
}