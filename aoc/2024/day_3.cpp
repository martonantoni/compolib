#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;


ll parseNum(string_view& s, char expectedEnd)
{
    ll num = 0;
    while(!s.empty())
    {
        char c = s.front();
        if (c < '0' || c > '9')
        {
            if (c == expectedEnd)
            {
                s.remove_prefix(1);
                return num;
            }
            return 0;
        }
        num = num * 10 + c - '0';
        s.remove_prefix(1);
    }
    return 0;
}

ll parseMul(string_view& s)
{
    if (!s.starts_with("mul("))
    {
        s.remove_prefix(1);
        return 0;
    }
    s.remove_prefix(4);
    ll a = parseNum(s, ',');
    if (!a)
        return 0;
    ll b = parseNum(s, ')');
    return a * b;
}

void solve(bool first)
{
    ll sum = 0;
    bool enabled = true;
    for (auto& l : ls)
    {
        string_view s = l.txt;

        while (s.size() >= 4)
        {
            if (!first)
            {
                if (s.starts_with("do()"))
                {
                    enabled = true;
                    s.remove_prefix(4);
                    continue;
                }
                if (s.starts_with("don't()"))
                {
                    enabled = false;
                    s.remove_prefix(7);
                    continue;
                }
            }
            ll r = parseMul(s);
            if(enabled)
                sum += r;
        }
    }
    P("Result: {}\n", sum);
}

