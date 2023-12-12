#include "aoc.h"
#include "bmp.h"


const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

void print_vll(vector<ll>& nums)
{
    for (auto x : nums)
        P("%lld ", x);
}

void print_f(const string& f)
{
    P("         ");
    for (auto c : f)
        P("%c ", c);
    P("\n");
}

ll process_bf(string& f, vector<ll> ds)
{
    P("%s ", f.c_str());
    ll res = 0;
    size_t fs = f.size();
    vector<size_t> qpos;
    for (size_t i = 0; i < fs; ++i)
        if (f[i] == '?')
            qpos.emplace_back(i);

    ll qs = (ll)qpos.size();
    ll top = 1ll << qs;
//     P("\n\nF: %s\n", f.c_str());
    FOR(i, top)
    {
        for (ll j = 0; j < qs; ++j)
        {
            if (i & (1ll << j))
                f[qpos[j]] = '#';
            else
                f[qpos[j]] = '.';
        }
        
        auto k = ds.begin();
        char prev = '.';
        bool matches = true;
        int len = 0;
        for (size_t j = 0; j < fs + 1; ++j)
        {
            if (j < fs && f[j] == '#')
            {
                ++len;
            }
            else
            {
                if (prev == '#')
                {
                    if (k == ds.end())
                    {
                        matches = false;
                        break;
                    }
                    if (len != *k)
                    {
                        matches = false;
                        break;
                    }
                    len = 0;
                    ++k;
                }
            }

            prev = f[j];
        }
        if (matches && k == ds.end())
        {
//             P("match: %s\n", f.c_str());
            ++res;
        }
    }
//     P("total: %lld\n", res);

    print_vll(ds);
    P("---> %lld\n", res);

    return res;
}

ll process(string& f, vector<ll> ds)
{
    ll res = 0;
    size_t fs = f.size();
    vector<vector<ll>> poss_lens(fs);
    for (size_t i = 0; i < fs; ++i)
    {
        if (f[i] == '.')
        {
            poss_lens[i].emplace_back(0);
            continue;
        }
        if (f[i] == '#' || f[i]=='?')
        {
            if (f[i] == '?')
                poss_lens[i].emplace_back(0);
            ll len = 1;
            for (size_t j = i + 1; j <= fs; ++j)
            {
                if (j == fs || f[j] == '.')
                {
                    poss_lens[i].emplace_back(len);
                    break;
                }
                if (f[j] == '?')
                {
                    poss_lens[i].emplace_back(len);
                }
                ++len;
            }
        }
    }
    vector<ll> counts_for_next_start(fs + 2, 0);
    vector<ll> next;
    next.reserve(fs + 2);
    counts_for_next_start[0] = 1ll;
    for(auto x: ds)
    {
        next.resize(fs + 2, 0);
        ll s = 0;
        for (size_t start_pos = 0; start_pos < fs; ++start_pos)
        {
            s += counts_for_next_start[start_pos];
            if (find(ALL(poss_lens[start_pos]), x) != poss_lens[start_pos].end())
            {
                next[start_pos + x + 1] = s;
            }
            if (poss_lens[start_pos][0] != 0)
            {
                s = 0;
            }
        }
        counts_for_next_start = move(next);
    }
    res = counts_for_next_start[fs] + counts_for_next_start[fs+1];
    for (size_t i = fs - 1; i != 0 && poss_lens[i][0]==0; --i)
    {
        res += counts_for_next_start[i];
    }
    return res;
}



// idea from https://github.com/dhanak/competitive-coding/blob/master/aoc2023/day12.jl
ll process_nicer(string& f, vector<ll> ds)   
{
    f = "."s + f + "."s;
//     print_f(f);
    vector<ll> ways_started_at(f.size() + 1), next(f.size() + 1);
    ways_started_at[0] = 1;
    for (auto len : ds)
    {
        fill(ALL(next), 0);
        ll ways_sum = ways_started_at[0];
        for (int from = 1; from < f.size(); ++from)
        {
            ways_sum += ways_started_at[from];
            if (int to = from + len; f[from - 1] != '#' && to <= f.size() && f[to] != '#' && string_view(f.c_str() + from, len).find('.') == string_view::npos)
                next[from + len + 1] = ways_sum;
            if (f[from] == '#')
                ways_sum = 0;
        }
//         P("%lld -----> ", len);
//         print_vll(next);
//         P("\n");
        swap(next, ways_started_at);
    }
    ll res = 0;
    for (size_t i = f.size(); i > 0 && f[i] != '#'; --i)
        res += ways_started_at[i];
    return res;
}

void solve(bool first)
{
    ll res = 0;
    for (auto& l : ls)
    {
        string& field = l.s[0];
        l.i.erase(l.i.begin());
        if (!first)
        {
            field = field + "?" + field + "?" + field + "?" + field + "?" + field;
            vector<ll> x = l.i;
            l.i.insert(l.i.end(), ALL(x));
            l.i.insert(l.i.end(), ALL(x));
            l.i.insert(l.i.end(), ALL(x));
            l.i.insert(l.i.end(), ALL(x));
        }
        res += process_nicer(field, l.i);
    }
    P("%lld\n", res);
}


// void solveSecond()
// {
// }

// void solve(bool first) { first ? solveFirst() : solveSecond(); }
