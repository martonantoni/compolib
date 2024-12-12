#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

ll calcVSides(vector<cPosition>& ps)
{
    ll sides = 0;
    unordered_map<ll, vector<ll>> rows;
    for (auto p : ps)
    {
        rows[p.row].push_back(p.col);
    }
    for (auto& [r, cols] : rows)
    {
        sort(ALL(cols));
        ++sides;
        ll prev = cols[0];
        for (ll i = 1; i < cols.size(); ++i)
        {
            if (cols[i] != prev + 1)
            {
                ++sides;
            }
            prev = cols[i];
        }
    }
    return sides;
}

ll calcHSides(vector<cPosition>& ps)
{
    ll sides = 0;
    unordered_map<ll, vector<ll>> cols;
    for (auto p : ps)
    {
        cols[p.col].push_back(p.row);
    }
    for (auto& [c, rows] : cols)
    {
        sort(ALL(rows));
        ++sides;
        ll prev = rows[0];
        for (ll i = 1; i < rows.size(); ++i)
        {
            if (rows[i] != prev + 1)
            {
                ++sides;
            }
            prev = rows[i];
        }
    }
    return sides;
}

ll calc(cImage<char>& img, cPosition p)
{
    ll perim = 0, area = 0;

    char c = img[p];
    vector<cPosition> border(1, p);

    vector<cPosition> sides[4];

    while (!border.empty())
    {
        cPosition p = border.back();
        border.pop_back();
        if (img[p] == c)
        {
            img[p] = '.';
            area++;
            for (auto [si, d] : views::enumerate(neighbour4Positions))
            {
                cPosition n = p + d;
                if (img.isValidPos(n) && img[n] == c)
                {
                    border.push_back(n);
                }
                else
                {
                    if (!img.isValidPos(n) || img[n] != '.')
                    {
                        if (is_first_part)
                            ++perim;
                        else
                            sides[si].push_back(p);
                    }
                }
            }
        }
    }

    if (!is_first_part)
    {
        perim += calcVSides(sides[0]);
        perim += calcVSides(sides[2]);
        perim += calcHSides(sides[1]);
        perim += calcHSides(sides[3]);
    }

    for (auto p : img.allPos())
    {
        if (img[p] == '.')
        {
            img[p] = '#';
        }
    }

//    P("char: {} perim: {}, area: {}", c, perim, area);

    return perim * area;
}

void solve(bool first)
{
    ll res = 0;

    auto img = loadImage(ls);

    for (auto p : img.allPos())
    {
        if (img[p] != '#')
        {
            res += calc(img, p);
        }
    }

    RESULT(res);
}