#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* main_delimeters = " ,{}=:";
bool main_allow_empty_fields = false;

struct cDir
{
    ll var;
    bool is_less;
    ll val;
    string target;
};

struct cRule
{
    vector<cDir> dirs; // x/m/a/s, target
    string name;
};

// px{a<2006:qkq,m>2090:A,rfg}
// px     a<2006 qkq    m>2090 A     rfg

unordered_map<string, cRule> createRules()
{
    unordered_map<string, cRule> rules;
    for (auto& l : blocks[0])
    {
        cRule& rule = rules[l.s[0]];
        rule.name = l.s[0];
        l.s.insert(l.s.end() - 1, "s>-1"s);
        ll pn = l.s.size();
        for (ll i = 1; i < pn; i += 2)
        {
            cDir& dir = rule.dirs.emplace_back();
            dir.var = "xmas"s.find(l.s[i][0]);
            dir.is_less = l.s[i][1] == '<';
            dir.val = stol(l.s[i].substr(2));
            dir.target = l.s[i + 1];
        }
    }
    return rules;
}

void solveFirst()
{
    auto rules = createRules();
    ll res = 0;
    for(auto& l: blocks[1])
    {
        auto ratings = l.i | views::drop(1) | views::stride(2);
        string rule_name = "in"s;
        for (;;)
        {
            for (auto& dir : rules[rule_name].dirs)
            {
                if (dir.is_less)
                {
                    if (dir.val > ratings[dir.var])
                    {
                        rule_name = dir.target;
                        break;
                    }
                }
                else
                {
                    if (dir.val < ratings[dir.var])
                    {
                        rule_name = dir.target;
                        break;
                    }
                }
            }
            if (rule_name == "A")
            {
                res += ratings[0] + ratings[1] + ratings[2] + ratings[3];
                break;
            }
            if (rule_name == "R")
            {
                break;
            }
        }
    }
    P("result: %lld", res);
}

void solveSecond()
{
    ll res = 0;
    auto rules = createRules();
    using cPart = array<pair<ll, ll>, 4>;
    auto size = [](pair<ll, ll> r) { return r.second >= r.first ? r.second - r.first + 1 : 0; };
    auto whole_size = [&](cPart& part) -> ll
        {
            ll s = 1;
            FOR(i, 4)
                s *= size(part[i]);
            return s;
        };
    auto split = [](cPart& part, cDir dir) -> pair<cPart, cPart> // going, remaining
        {
            cPart going = part;
            cPart remaining = part;
            pair<ll, ll>& going_sp = going[dir.var];
            pair<ll, ll>& remaining_sp = remaining[dir.var];
            if (dir.is_less)
            {
                going_sp.second = min(going_sp.second, dir.val - 1);
                remaining_sp.first = max(remaining_sp.first, dir.val);
            }
            else
            {
                going_sp.first = max(going_sp.first, dir.val + 1);
                remaining_sp.second = min(remaining_sp.second, dir.val);
            }
            return { going, remaining };
        };
    vector<pair<string, cPart>> parts;
    cPart initial;
    FOR(i, 4)
    {
        initial[i].first = 1;
        initial[i].second = 4000;
    }
    parts.emplace_back("in"s, initial);
    while (!parts.empty())
    {
        auto [rule_name, part] = parts.back();
        parts.pop_back();
        if (rule_name == "A")
        {
            res += size(part[0]) * size(part[1]) * size(part[2]) * size(part[3]);
        }
        else if (rule_name != "R")
        {
            cRule& rule = rules[rule_name];
            for (auto& dir : rule.dirs)
            {
                auto [going, remaining] = split(part, dir);
                if (whole_size(going) > 0)
                {
                    parts.emplace_back(dir.target, going);
                }
                if (whole_size(remaining) > 0)
                {
                    part = remaining;
                }
                else
                    break;
            }
        }
    }

    P("result: %lld", res);
}

 void solve(bool first) { first ? solveFirst() : solveSecond(); }
