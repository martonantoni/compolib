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
    ll target;
};

struct cRule
{
    vector<cDir> dirs; // x/m/a/s, target
    ll final_target;
    string name;
};

// px{a<2006:qkq,m>2090:A,rfg}
// px     a<2006 qkq    m>2090 A     rfg

void solveFirst()
{
    unordered_map<ll, cRule> rules;
    unordered_map<string, ll> rule_names;
    constexpr ll ACC = -1;
    constexpr ll REJ = -2;
    rule_names["A"s] = ACC;
    rule_names["R"s] = REJ;
    ll res = 0;
    bool second_part = false;
    auto name_idx = [&](string& name) -> ll
        {
            ll& name_idx = rule_names[name];
            if (name_idx == 0)
            {
                name_idx = rule_names.size() - 1ll;
            }
            return name_idx;
        };
    auto next = [&](array<ll, 4>& p, cRule& rule) -> ll
        {
            for (auto& dir : rule.dirs)
            {
                ll value = p[dir.var];
                if (dir.is_less)
                {
                    if (dir.val > value)
                        return dir.target;
                }
                else
                {
                    if (dir.val < value)
                        return dir.target;
                }
            }
            return rule.final_target;
        };

    for (auto& l : ls)
    {
        if (l.is_empty)
        {
            second_part = true;
            continue;
        }
        if (!second_part)
        {
            cRule& rule = rules[name_idx(l.s[0])];
            rule.name = l.s[0];
            ll pn = l.s.size();
            for (ll i = 1; i < pn - 1; i+=2)
            {
                cDir& dir = rule.dirs.emplace_back();
                dir.var = "xmas"s.find(l.s[i][0]);
                dir.is_less = l.s[i][1] == '<';
                dir.val = stol(l.s[i].substr(2));
                dir.target = name_idx(l.s[i + 1]);
            }
            rule.final_target = name_idx(l.s[pn - 1]);
        }
        else
        {
            array<ll, 4> p;
            FOR(i, 4)
                p[i] = l.i[i*2+1];
            
            ll ri = rule_names["in"s];
            P("%lld, %lld, %lld, %lld -> in", p[0], p[1], p[2], p[3]);

            for (;;)
            {
                ri = next(p, rules[ri]);
                if (ri == ACC)
                {
                    PC(" -> ACCEPTED");
                    res += p[0] + p[1] + p[2] + p[3];
                    break;
                }
                if (ri == REJ)
                {
                    PC(" -> REJ");
                    break;
                }
                PC(" -> %s", rules[ri].name.c_str());
            }
        }
    }
    P("result: %lld", res);
}

void solveSecond()
{
    unordered_map<ll, cRule> rules;
    unordered_map<string, ll> rule_names;
    constexpr ll ACC = -1;
    constexpr ll REJ = -2;
    rule_names["A"s] = ACC;
    rule_names["R"s] = REJ;
    ll res = 0;
    bool second_part = false;
    auto name_idx = [&](const string& name) -> ll
        {
            ll& name_idx = rule_names[name];
            if (name_idx == 0)
            {
                name_idx = rule_names.size() - 1ll;
            }
            return name_idx;
        };
    auto next = [&](array<ll, 4>& p, cRule& rule) -> ll
        {
            for (auto& dir : rule.dirs)
            {
                ll value = p[dir.var];
                if (dir.is_less)
                {
                    if (dir.val > value)
                        return dir.target;
                }
                else
                {
                    if (dir.val < value)
                        return dir.target;
                }
            }
            return rule.final_target;
        };

    for (auto& l : ls)
    {
        if (l.is_empty)
        {
            break;
        }
        cRule& rule = rules[name_idx(l.s[0])];
        rule.name = l.s[0];
        ll pn = l.s.size();
        for (ll i = 1; i < pn - 1; i += 2)
        {
            cDir& dir = rule.dirs.emplace_back();
            dir.var = "xmas"s.find(l.s[i][0]);
            dir.is_less = l.s[i][1] == '<';
            dir.val = stol(l.s[i].substr(2));
            dir.target = name_idx(l.s[i + 1]);
        }
        rule.final_target = name_idx(l.s[pn - 1]);
    }

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
    vector<pair<ll, cPart>> parts;
    cPart initial;
    FOR(i, 4)
    {
        initial[i].first = 1;
        initial[i].second = 4000;
    }
    parts.emplace_back(name_idx("in"s), initial);
    while (!parts.empty())
    {
        auto [ri, part] = parts.back();
        parts.pop_back();
        if (ri == ACC)
        {
            res += size(part[0]) * size(part[1]) * size(part[2]) * size(part[3]);
        }
        else if (ri != REJ)
        {
            cRule& rule = rules[ri];
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
            if (whole_size(part))
            {
                parts.emplace_back(rule.final_target, part);
            }
        }
    }

    P("result: %lld", res);
}

 void solve(bool first) { first ? solveFirst() : solveSecond(); }
