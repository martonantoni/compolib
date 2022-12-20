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
#define PERF_LOG(...) fprintf(stderr, __VA_ARGS__)
#define D(expression) expression
#else
char* crash_please = (char*)42;
#define ASSERT(x) 
//if(!(x)) { printf("%s failed",#x); *crash_please=33; }
#ifdef _WIN32
#define PERF_LOG(...) fprintf(stderr, __VA_ARGS__)
#else
#define PERF_LOG(...)
#endif
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
        PERF_LOG("\n--- Elapsed %llu.%llu ms in %s ---\n", (unsigned long long)Elapsed.count() / 1000, (unsigned long long)Elapsed.count() % 1000, mName);
    }
    void waypoint()
    {
        auto EndTime = std::chrono::high_resolution_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - mStartTime);
        PERF_LOG("+ %llu.%llu ms: ", (unsigned long long)Elapsed.count() / 1000, (unsigned long long)Elapsed.count() % 1000);
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

vector<ll> orig_nums;
int n;
int zero_i;

void prepare()
{
    cFastFileReader in("in.txt");
    for (auto line : in)
    {
        int v = atoi(line.Data);
        if (v == 0)
            zero_i = (int)orig_nums.size();
        orig_nums.emplace_back(v);
    }
    n = (int)orig_nums.size();
}

ll mod_substract(ll x, ll y, ll m)
{
    return ((x - y) % m) + ((x >= y) ? 0 : m);
}

void Solve()
{
    vector<int> positions(n);
    iota(ALL(positions), 0);
    auto adjustRange = [&](int from, int to, int offset)
    {
        for (int i = 0; i < n; ++i)
        {
            if (positions[i] >= from && positions[i] < to)
                positions[i] += offset;
        }
    };
    auto getNumberAtPos = [&](int pos)
    {
        auto i = find(ALL(positions), pos);
        return orig_nums[i - positions.begin()];
    };
    int m = n - 1;
    for (int i = 0; i < n; ++i)
    {
        int& p = positions[i];
        int new_p = orig_nums[i] > 0 ?
            (p + orig_nums[i]) % m :
            mod_substract(p, -orig_nums[i], m);
        if(new_p == p)
            continue;
        if (new_p < p)
        {
            adjustRange(new_p, p, 1);
        }
        else
        {
            adjustRange(p + 1, new_p + 1, -1);
        }
        p = new_p;

    }
    int zero_pos = positions[zero_i];

    printf("zero pos: %d\n", zero_pos);
    printf("1000th: %lld\n", getNumberAtPos((zero_pos + 1000) % n));
    printf("2000th: %lld\n", getNumberAtPos((zero_pos + 2000) % n));
    printf("3000th: %lld\n", getNumberAtPos((zero_pos + 3000) % n));

    printf("\nresult part 1: %lld\n",
        getNumberAtPos((zero_pos + 1000) % n)
        + getNumberAtPos((zero_pos + 2000) % n)
        + getNumberAtPos((zero_pos + 3000) % n));
}

static constexpr ll key = 811589153;

void Solve2()
{
    vector<int> positions(n);
    iota(ALL(positions), 0);
    auto adjustRange = [&](int from, int to, int offset)
    {
        for (int i = 0; i < n; ++i)
        {
            if (positions[i] >= from && positions[i] < to)
                positions[i] += offset;
        }
    };
    auto getNumberAtPos = [&](int pos)
    {
        auto i = find(ALL(positions), pos);
        return orig_nums[i - positions.begin()];
    };
    int m = n - 1;


    for (auto& v : orig_nums)
        v *= key;

    for (int mix = 0; mix < 10; ++mix)
    {
        for (int i = 0; i < n; ++i)
        {
            int& p = positions[i];
            int new_p = orig_nums[i] > 0 ?
                (p + orig_nums[i]) % m :
                mod_substract(p, -orig_nums[i], m);
            if (new_p == p)
                continue;
            if (new_p < p)
            {
                adjustRange(new_p, p, 1);
            }
            else
            {
                adjustRange(p + 1, new_p + 1, -1);
            }
            p = new_p;
        }
    }
    int zero_pos = positions[zero_i];

    printf("zero pos: %d\n", zero_pos);
    printf("1000th: %lld\n", getNumberAtPos((zero_pos + 1000) % n));
    printf("2000th: %lld\n", getNumberAtPos((zero_pos + 2000) % n));
    printf("3000th: %lld\n", getNumberAtPos((zero_pos + 3000) % n));

    printf("\nresult part 2: %lld\n",
        getNumberAtPos((zero_pos + 1000) % n)
        + getNumberAtPos((zero_pos + 2000) % n)
        + getNumberAtPos((zero_pos + 3000) % n));

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
        Solve();
        Solve2();
 //       printf("%s\n", Solve() ? "YES" : "NO");
//        printf("result: %lld\n", result);
        //         RI(verdict);
        //         if (verdict == -1)
        //             break;
    }
}
