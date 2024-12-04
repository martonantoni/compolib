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

bool isXMAS(auto& img, auto pos, auto dir)
{
    for (char c: "XMAS"s)
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
    if (img.at(center) != 'A')
        return false;

    cPosition a1 = center + direction_NE;
    cPosition a2 = center + direction_SW;

    cPosition b1 = center + direction_NW;
    cPosition b2 = center + direction_SE;

    if (!img.isValidPos(a1) || !img.isValidPos(a2) || !img.isValidPos(b1) || !img.isValidPos(b2))
        return false;

    if ((img[a1] == 'M' && img[a2] == 'S') || (img[a1] == 'S' && img[a2] == 'M'))
    {
        if ((img[b1] == 'M' && img[b2] == 'S') || (img[b1] == 'S' && img[b2] == 'M'))
            return true;
    }
    return false;
}

void solveFirst()
{
    ll sum = 0;

    cImage img = loadImage(ls);
    for (auto p : img.allPos())
    {
        for (auto dir : neighbour8Positions)
        {
            if (isXMAS(img, p, dir))
                ++sum;
        }
    }

    P("Result: {}\n", sum);
}

void solveSecond()
{
    ll sum = 0;

    cImage img = loadImage(ls);
    for (auto p : img.allPos())
    {
        if (isMAS(img, p))
            ++sum;
    }
    P("Result: {}\n", sum);
}
