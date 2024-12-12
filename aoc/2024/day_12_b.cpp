#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

ll calc(cImage<char>& img, cPosition p)
{
    ll perim = 0, area = 0, side_count = 0;
    unordered_map<ll, set<ll>> sides[4];
    char c = img[p];
    vector<cPosition> frontier(1, p);

    while (!frontier.empty())
    {
        cPosition p = frontier.back();
        frontier.pop_back();
        if (img[p] == c)
        {
            img[p] = '.';
            ++area;
            for (auto [si, d] : views::enumerate(neighbour4Positions))
            {
                cPosition n = p + d;
                if (img.isValidPos(n) && img[n] == c)
                    frontier.push_back(n);
                else
                {
                    if (!img.isValidPos(n) || img[n] != '.')
                    {
                        ++perim;
                        sides[si][p.component(si % 2)].insert(p.component(1 - si % 2));
                    }
                }
            }
        }
    }

    for (auto& side : sides)
    {
        for (auto& v : side | views::values)
        {
            ++side_count;
            for (auto [a, b] : v | views::pairwise)
                if (b - a != 1)
                    ++side_count;
        }
    }

    for (auto p : img.allPos())
        if (img[p] == '.')
            img[p] = '#';

    return (is_first_part ? perim : side_count) * area;
}

void solve(bool first)
{
    ll res = 0;
    auto img = loadImage(ls);

    for (auto p : img.allPos())
        if (img[p] != '#')
            res += calc(img, p);

    RESULT(res);
}