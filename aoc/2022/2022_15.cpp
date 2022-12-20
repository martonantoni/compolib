#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

#include <iostream>
#include <stdio.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <string>
#include <set>
#include <tuple>
#include <functional>
#include <cstring>
#include <deque>
#include <array>
#include <map>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <cstddef>
#include <cinttypes>
#include <regex>
#include <string>
#include <type_traits>
#include <variant>

#undef max
#undef min
#undef I

#ifdef __GNUC__ 
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

#ifdef _DEBUG
#define ASSERT(x) if(!(x)) __debugbreak()
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#define D(expression) expression
#else
char* crash_please = (char*)42;
#define ASSERT(x) 
//if(!(x)) { printf("%s failed",#x); *crash_please=33; }
#define LOG(...) fprintf(stderr, __VA_ARGS__)
//#define LOG(...)
#define D(expression)
#endif

#include <chrono>

class cLogPerformance_Guard
{
    std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime = std::chrono::high_resolution_clock::now();
    const char* mName;
public:
    cLogPerformance_Guard(const char* Name) : mName(Name) {}
    ~cLogPerformance_Guard()
    {
        auto EndTime = std::chrono::high_resolution_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - mStartTime);
        LOG("\n--- Elapsed %llu.%llu ms in %s ---\n", (unsigned long long)Elapsed.count() / 1000, (unsigned long long)Elapsed.count() % 1000, mName);
    }
    void waypoint()
    {
        auto EndTime = std::chrono::high_resolution_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - mStartTime);
        LOG("+ %llu.%llu ms: ", (unsigned long long)Elapsed.count() / 1000, (unsigned long long)Elapsed.count() % 1000);
    }
} perf_guard("main");

using namespace std;
using namespace std::chrono;
using namespace std::string_literals;
using ull = unsigned long long;
using ll = long long;
#define RI(var_name) int var_name; scanf("%d", &var_name);
#define RIV(var_name, size) vector<int> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%d", &item_of_##var_name);
#define RII(var_name1, var_name2) int var_name1, var_name2; scanf("%d %d", &var_name1, &var_name2);
#define RIII(var_name1, var_name2, var_name3) int var_name1, var_name2, var_name3; scanf("%d %d %d", &var_name1, &var_name2, &var_name3);
#define RIIII(var_name1, var_name2, var_name3, var_name4) int var_name1, var_name2, var_name3, var_name4; scanf("%d %d %d %d", &var_name1, &var_name2, &var_name3, &var_name4);
#define RL(var_name) ll var_name; scanf("%lld", &var_name);
#define RLV(var_name, size) vector<ll> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%lld", &item_of_##var_name);
#define RLUV(var_name, size) vector<ull> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%llu", &item_of_##var_name);
#define RLL(var_name1, var_name2) ll var_name1, var_name2; scanf("%lld %lld", &var_name1, &var_name2);
#define RLLL(var_name1, var_name2, var_name3) ll var_name1, var_name2, var_name3; scanf("%lld %lld %lld", &var_name1, &var_name2, &var_name3);
#define RLLLL(var_name1, var_name2, var_name3, var_name4) ll var_name1, var_name2, var_name3, var_name4; scanf("%lld %lld %lld %lld", &var_name1, &var_name2, &var_name3, &var_name4);
#define RD(var_name) double var_name; scanf("%lf", &var_name);
#define RDV(var_name, size) vector<double> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%lf", &item_of_##var_name);
#define RDD(var_name1, var_name2) double var_name1, var_name2; scanf("%lf %lf", &var_name1, &var_name2);
#define RDDD(var_name1, var_name2, var_name3) double var_name1, var_name2, var_name3; scanf("%lf %lf %lf", &var_name1, &var_name2, &var_name3);
#define ALL(cont) cont.begin(), cont.end()
#define FOR(var, max_value) for(decltype(max_value) var=0;var<max_value;++var)

constexpr ll mod = 1'000'000'007;

#include "aoc_common.h"

struct cSensor
{
    ll x, y;
    ll bx, by;
    ll d;
};


vector<cSensor> ss;
vector<pair<ll,ll>> bs; // x, y

//Sensor at x=3088287, y=2966967: closest beacon is at x=3340990, y=2451747
// 0     1  2   3      4   5       6       7      8  9 10 11      12   13 
void prepare()
{
    cFastFileReader in("in.txt");
    for (auto line : in)
    {
        auto ts = line.toStringVector(" =:,", false);
        cSensor s;
        s.x = stol(ts[3]);
        s.y = stol(ts[5]);
        s.bx = stol(ts[11]);
        s.by = stol(ts[13]);
        s.d = abs(s.x - s.bx) + abs(s.y - s.by);
        ss.emplace_back(s);
        bs.emplace_back(s.bx, s.by);
    }
//    sort(ALL(ss), [](auto& l, auto& r) { return l.x < r.x; });
    sort(ALL(bs));
    bs.erase(unique(ALL(bs)), bs.end());
}

void Solve()
{
    static constexpr ll y = 2000000;
//    static constexpr ll y = 10;
    vector<pair<ll, int>> ps;
    ll result = 0;
    for (auto& s : ss)
    {
        ll remaining = s.d - abs(y - s.y);
        if (remaining > 0)
        {
            ps.emplace_back(s.x - remaining, 1);
            ps.emplace_back(s.x + remaining + 1, -1);
        }
    }
    for (auto [bx, by] : bs)
    {
        if (by == y)
            --result;
    }
    sort(ALL(ps));
    ll last = 0;
    int o = 0;
    for (auto [pos, onoff] : ps)
    {
        if (o > 0)
            result += pos - last;
        o += onoff;
        last = pos;
    }
    printf("result part 1: %lld\n", result);
}

void Solve2()
{
//     ll top = ss[0].y - ss[0].d, bottom = ss[0].y + ss[0].d;
//     for (auto& s : ss)
//     {
//         top = min(s.y - s.d, top);
//         bottom = max(s.y + s.d, bottom);
//     }
//     printf("top: %lld\nbottom: %lld\n", top, bottom);
// 
     vector<pair<ll, int>> ps;
//     ll result = 0;

    ll top = 0, bottom = 4000000;

    for (ll y = top; y <= bottom; ++y)
    {
        ps.clear();
        for (auto& s : ss)
        {
            ll remaining = s.d - abs(y - s.y);
            if (remaining > 0)
            {
                ps.emplace_back(s.x - remaining, 1);
                ps.emplace_back(s.x + remaining + 1, -1);
            }
        }
//         for (auto [bx, by] : bs)
//         {
//             if (by == y)
//                 --result;
//         }
        sort(ALL(ps));
        ll last = 0;
        int o = 0;
        for (auto [pos, onoff] : ps)
        {
//             if (o > 0)
//                 result += pos - last;
            o += onoff;
            if (o == 0 && pos >= 0 && pos <= 4000000)
            {
                printf("found: %lld, %lld, result (part 2): %lld\n", pos, y, pos * 4000000ll + y);
            }
            last = pos;
        }
    }
 //   printf("result part 1: %lld\n", result);
}

void Solve3()
{
    ll top = ss[0].y - ss[0].d, bottom = ss[0].y + ss[0].d;
    for (auto& s : ss)
    {
        top = min(s.y - s.d, top);
        bottom = max(s.y + s.d, bottom);
    }
    printf("top: %lld\nbottom: %lld\n", top, bottom);

    vector<pair<ll, int>> ps;
    ll result = 0;

    for (ll y = top - 1; y <= bottom + 1; ++y)
    {
        ps.clear();
        for (auto& s : ss)
        {
            ll remaining = s.d - abs(y - s.y);
            if (remaining > 0)
            {
                ps.emplace_back(s.x - remaining, 1);
                ps.emplace_back(s.x + remaining + 1, -1);
            }
        }
        for (auto [bx, by] : bs)
        {
            if (by == y)
                --result;
        }
        sort(ALL(ps));
        ll last = 0;
        int o = 0;
        for (auto [pos, onoff] : ps)
        {
            if (o > 0)
                result += pos - last;
            o += onoff;
            last = pos;
        }
    }
    printf("all locations where sensors cannot be: %lld\n", result);
}

int Init()
{
    constexpr int t = 1;
 //   RI(t);
    return t;
}

int main()
{
    int t = Init();
    prepare();
    for (int tc = 1; tc <= t; ++tc)
    {
//         printf("Case #%d: ", tc);
//         Solve();
//         Solve2();
        Solve3();
 //       printf("%s\n", Solve() ? "YES" : "NO");
//        printf("result: %lld\n", result);
        //         RI(verdict);
        //         if (verdict == -1)
        //             break;
    }
}
