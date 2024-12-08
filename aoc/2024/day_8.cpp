#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,:";
bool main_allow_empty_fields = false;

void solve(bool first)
{
    auto img = loadImage(ls);

    ll res = 0;
    unordered_map<int, vector<cPosition>> as;
    unordered_set<cPosition> antis;
    auto add = [&](cPosition p)
        {
            if (img.isValidPos(p))
            {
                antis.insert(p);
                return true;
            }
            return false;
        };

    for (auto [p, c] : img.allPosV())
    {
        if (c != '.')
        {
            auto& v = as[c];
            for (auto q : v)
            {
                auto diff = q - p;
                if (first)
                {
                    add(p - diff);
                    add(q + diff);
                }
                else
                {
                    auto i = p;
                    while (add(i))
                        i = i - diff;
                    i = q;
                    while (add(i))
                        i = i + diff;
                }
            }
            v.push_back(p);
        }
    }
    P("Result: {}\n", antis.size());
}
