#include "aoc.h"
#include "bmp.h"

const char* main_delimeters = " =(),";
bool main_allow_empty_fields = false;

ll gcd(ll a, ll b)
{
    while (b) b ^= a ^= b ^= a %= b;
    return a;
}

ll lcm(ll a, ll b)
{
    return (a / gcd(a, b)) * b;
}

ll lcm(const vector<ll>& nums)
{
    if (nums.empty())
        return 0;
    if (nums.size() == 1)
        return nums[0];
    ll res = lcm(nums[0], nums[1]);
    for (auto x : nums | views::drop(2))
        res = lcm(res, x);
    return res;
}

void solve(bool first)
{
    string& dirs = ls[0].txt;

    unordered_map<string, pair<string, string>> m;
    vector<string> starting_nodes;
    for(auto& line: ls | views::drop(2))
    {
        m[line.s[0]] = make_pair(line.s[1], line.s[2]);
        if (line.s[0][2] == 'A')
            starting_nodes.emplace_back(line.s[0]);
    }
    auto count = [&](string pos, auto check)
    {
        ll res = 0;
        for (const char* p = dirs.c_str();; ++p)
        {
            if (!*p)
                p = dirs.c_str();
            ++res;
            pos = *p == 'L' ? m[pos].first : m[pos].second;
            if (check(pos))
                break;
        }
        return res;
    };

    vector<ll> steps;
    if (!first)
    {
        for (auto& pos : starting_nodes)
        {
            steps.emplace_back(count(pos, [](const string& p) { return p[2] == 'Z'; }));
        }
    }
    else
    {
        steps.emplace_back(count("AAA"s, [](const string& p) { return p == "ZZZ"; }));
    }

    P("%lld", lcm(steps));
}

// void solveSecond()
// {
// }

// void solve(bool first) { first ? solveFirst() : solveSecond(); }
