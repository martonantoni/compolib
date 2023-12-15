#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

void print_vll(vector<ll>& nums)
{
    for (auto x : nums)
        P("%lld ", x);
}

ll process(string& s)
{
    ll res = 0;
    for (auto& c : s)
    {
        res += c;
        res *= 17;
        res %= 256;
    }
    return res;
}

void solve(bool first)
{
    ll res = 0;
    vector<vector<pair<string, ll>>> boxes;
    boxes.resize(256);
    for (auto& l : ls)
    {
        for (auto& s : l.s)
        {
            if (first)
            {
                res += process(s);
            }
            else
            {
                cStringVector ps(s, "=-", false);
                ll h = process(ps[0]);
                auto& box = boxes[h];
                auto i = find_if(ALL(box), [&](const pair<string, ll>& b) { return b.first == ps[0]; });
                if (s[ps[0].length()] == '-')
                {
                    if (i != box.end())
                        box.erase(i);
                }
                else
                {
                    if (i != box.end())
                    {
                        i->second = atoll(ps[1].c_str());
                    }
                    else
                    {
                        box.emplace_back(ps[0], atoll(ps[1].c_str()));
                    }

                }
            }
        }
    }
    if (!first)
    {
        FOR(i, 256)
        {
            auto& box = boxes[i];
            for (ll j = 0; j < (ll)box.size(); ++j)
            {
                res += (i + 1) * (j + 1) * box[j].second;
            }
        }
    }
    P("result: %lld\n", res);
}


// void solveSecond()
// {
// }

// void solve(bool first) { first ? solveFirst() : solveSecond(); }
