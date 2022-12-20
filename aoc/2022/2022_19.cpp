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

int divRoundUp(int a, int b)
{
    return (a + b - 1) / b;
}

struct cRobot
{
    int cost[4] = { 0 };
};

struct cBlueprint
{
    cRobot r[4];
};

vector<cBlueprint> bps;

void prepare()
{
    cFastFileReader in("in.txt");
    for (auto line : in)
    {
        bps.emplace_back();
        auto& bp = bps.back();
        auto ts = line.toStringVector(" ").ToIntVector();
        bp.r[0].cost[0] = (int)ts[6];
        bp.r[1].cost[0] = (int)ts[12];
        bp.r[2].cost[0] = (int)ts[18];
        bp.r[2].cost[1] = (int)ts[21];
        bp.r[3].cost[0] = (int)ts[27];
        bp.r[3].cost[2] = (int)ts[30];
    }
}


int robotCaps[4] = { 0, 0, 0, 100 };

int getBest(cBlueprint& bp, int mins, array<int, 4> storage, array<int, 4> robots, int next_robot)
{
    if (mins <= 1)
        return storage[3] + robots[3]*mins;

    int wait_mins = 0;
    for (int i = 0; i < 3; ++i)
    {
        if (bp.r[next_robot].cost[i] > 0)
        {
            if (robots[i] == 0)
                return 0;
            wait_mins = max(wait_mins, divRoundUp(bp.r[next_robot].cost[i] - storage[i], robots[i]));
        }
    }
    ++wait_mins; // build time
    wait_mins = min(wait_mins, mins);
    for (int i = 0; i < 4; ++i)
    {
        storage[i] += robots[i] * wait_mins;
        storage[i] -= bp.r[next_robot].cost[i];
    }
    ++robots[next_robot];

    int best = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (robots[i] < robotCaps[i])
        {
            best = max(best, getBest(bp, mins - wait_mins, storage, robots, i));
        }
    }
    return best;
}

int calc(cBlueprint& bp, int mins)
{
    int best = 0;
    for (int i = 0; i < 3; ++i)
    {
        robotCaps[i] = bp.r[0].cost[i];
        for (int j = 1; j < 4; ++j)
            robotCaps[i] = max(robotCaps[i], bp.r[j].cost[i]);
    }
    for (int i = 0; i < 4; ++i)
    {
        best = max(best, getBest(bp, mins, { 0, 0, 0, 0 }, { 1, 0, 0, 0 }, i));
    }
    return best;
}

void Solve()
{
    int result = 0;
    for (int i = 0; i < bps.size(); ++i)
    {
        int best = calc(bps[i], 24);
        printf("bp #%d best: %d geodes.\n", i+1, best);
        result += best * (i + 1);
    }
    printf("Result part 1: %d\n", result);
}

void Solve2()
{
    ll result = 1;
    for (int i = 0; i < 3; ++i)
    {
        ll best = calc(bps[i], 32);
        printf("bp #%d best: %lld geodes.\n", i + 1, best);
        result *= best;
    }
    printf("Result part 1: %lld\n", result);
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
