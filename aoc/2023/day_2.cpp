#include "aoc.h"

const char* main_delimeters = " ,";

void solve(bool first)
{
    ll r = 0;
    unordered_map<string, int> bag;
    bag["red"] = 12;
    bag["green"] = 13;
    bag["blue"] = 14;
    for (auto& l : ls)
    {
        cStringVector big_parts(l.txt, ":;");
        big_parts.erase(big_parts.begin());
        if (first)
        {
            bool failed = false;
            for (auto& rev : big_parts)
            {
                cStringVector ps(rev, " ,", false);
                unordered_map<string, int> rc;
                for (int i = 0; !failed && i < ps.size(); i += 2)
                {
                    if (bag[ps[i + 1]] < atoi(ps[i].c_str()))
                    {
                        failed = true;
                        break;
                    }
                }
            }
            if (!failed)
                r += l.idx + 1;
        }
        else
        {
            unordered_map<string, int> mins;
            for (auto& rev : big_parts)
            {
                cStringVector ps(rev, " ,", false);
                for (int i = 0;i < ps.size(); i += 2)
                {
                    mins[ps[i + 1]] = max(mins[ps[i + 1]], atoi(ps[i].c_str()));
                }
            }
            ll s = 1;
            for (auto& [col, c] : mins)
                s *= c;
            if (mins.size() < 3)
                s = 0;
            r += s;
        }
    }
    P("%lld\n", r);
}

