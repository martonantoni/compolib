#define _CRT_SECURE_NO_WARNINGS
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
#include <ranges>
#include <print>
#include <bit>

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
//#define LOG(...) fprintf(stderr, __VA_ARGS__)
#define LOG(...)
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

namespace rng = std::ranges;
namespace views = std::ranges::views;
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
#define FOR(var, max_value) for(remove_reference<remove_const<decltype(max_value)>::type>::type var=0;var<max_value;++var)

constexpr ll mod = 1'000'000'007;
//constexpr ull mod = 998244353;
//        998244353
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Solve()
{
    RII(n, m);
    vector<pair<int, int>> mv_ps;
    int mv = 0, mv_count = 0;
    FOR(r, n)
    {
        FOR(c, m)
        {
            RI(x);
            if (x > mv)
            {
                mv = x;
                mv_count = 1;
                mv_ps.clear();
                mv_ps.emplace_back(r, c);
            }
            else if (x == mv)
            {
                ++mv_count;
                mv_ps.emplace_back(r, c);
            }
        }
    }
    if (mv_count <= 2)
    {
        printf("%d\n", mv - 1);
        return;
    }
    sort(ALL(mv_ps));
    int selected_row = -1;
    for (int i = 1; i < mv_ps.size(); ++i)
    {
        if(mv_ps[i].first == mv_ps[i - 1].first)
        {
            if (selected_row == -1)
            {
                selected_row = mv_ps[i].first;
            }
            else if(selected_row != mv_ps[i].first)
            {
                printf("%d\n", mv);
                return;
            }
        }
    }
    sort(ALL(mv_ps), [](auto& a, auto& b) { return a.second < b.second; });
    int selected_col = -1;
    for (int i = 1; i < mv_ps.size(); ++i)
    {
        if (mv_ps[i].second == mv_ps[i - 1].second)
        {
            if (selected_col == -1)
            {
                selected_col = mv_ps[i].second;
            }
            else if (selected_col != mv_ps[i].second)
            {
                printf("%d\n", mv);
                return;
            }
        }
    }
    if (selected_row == -1 && selected_col == -1)
    {
        printf("%d\n", mv);
        return;
    }
    if(selected_row != -1 && selected_col != -1)
    {
        for(auto& p : mv_ps)
        {
            if (p.first == selected_row || p.second == selected_col)
            {
                printf("%d\n", mv - 1);
                return;
            }
        }
        printf("%d\n", mv);
        return;
    }
    int divergent = 0;
    for (auto& p : mv_ps)
    {
        if ((selected_row != -1 && p.first != selected_row) || (selected_col != -1 && p.second != selected_col))
        {
            ++divergent;
        }
    }
    if (divergent == 1)
    {
        printf("%d\n", mv - 1);
    }
    else
    {
        printf("%d\n", mv);
    }
}

bool SolveYN()
{
    return true;
}

int Init()
{
    RI(t);
    return t;
}

int main()
{
    int t = Init();
    for (int tc = 1; tc <= t; ++tc)
    {
        //    printf("Case #%d: ", tc);
        Solve();
//        printf("%s\n", SolveYN() ? "YES" : "NO");
    }
}

