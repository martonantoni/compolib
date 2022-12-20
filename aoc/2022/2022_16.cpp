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

struct cNode
{
    string name;
    ll rate;
    vector<int> to;

    vector<int> distances;
};

vector<cNode> nodes;
vector<int> good_valves;
int start_id;

void calcDistances(int start)
{
    deque<pair<int, int>> f;
    f.emplace_back(0, start);
    while (!f.empty())
    {
        auto [dist, idx] = f.front();
        f.pop_front();
        cNode& node = nodes[idx];
        if (node.distances[start] > dist)
        {
            node.distances[start] = dist;
            for (int to : node.to)
            {
                f.emplace_back(dist + 1, to);
            }
        }
    }
    //     for (auto& node : nodes)
    //     {
    //         printf("node %s, rate: %lld, distance: %d\n", node.name.c_str(), node.rate, node.distance);
    //     }
}

void prepare()
{
    nodes.resize(100);
    cFastFileReader in("in.txt");
    unordered_map<string, int> ids;
    auto get_id = [&](string& s)
    {
        auto i = ids.find(s);
        if (i == ids.end())
        {
            return ids[s] = (int)ids.size();
        }
        return i->second;
    };
    for (auto line : in)
    {
        auto ts = line.toStringVector(" =:;,", false);
        int id = get_id(ts[1]);
        cNode& node = nodes[id];
        node.name = ts[1];
        if (ts[1] == "AA")
            start_id = id;
        node.rate = stol(ts[5]);
        if (node.rate > 0)
        {
            good_valves.emplace_back(id);
        }
        for (int i = 10; i < ts.size(); ++i)
        {
            node.to.emplace_back(get_id(ts[i]));
        }
    }
    nodes.resize(ids.size());
    for (auto& node : nodes) node.distances.resize(nodes.size(), 100);
    for (int i = 0; i < (int)ids.size(); ++i)
    {
        calcDistances(i);
    }
}

ll calcBest(int i, int mins, int visited)
{
    if (mins <= 1)
        return 0;
    cNode& node = nodes[i];
    if (node.rate > 0)
    {
        --mins; // opening the valve, but not at the start
    }
    ll result = node.rate * mins;
    ll sub_best = 0;
    for (int j = 0; j < (int)good_valves.size(); ++j)
    {
        if ((visited & (1 << j)) == 0)
        {
            int to = good_valves[j];
            sub_best = max(sub_best, calcBest(to, mins - node.distances[to], visited | (1<<j)));
        }
    }
    return result + sub_best;
}


void Solve()
{
    ll result = calcBest(start_id, 30, 0);
    printf("result part 1: %lld\n", result);
}

int GetNumberOfSetBits(uint32_t i)
{
    i = i - ((i >> 1) & 0x55555555u);
    i = (i & 0x33333333u) + ((i >> 2) & 0x33333333u);
    return (((i + (i >> 4)) & 0x0F0F0F0Fu) * 0x01010101u) >> 24;
}

void Solve2()
{
    ll best = 0;
    for (int m = 0; m < (1 << 16); ++m)
    {
        int bcount = GetNumberOfSetBits((uint32_t)m);
        if (bcount >= 5 && bcount <= 10)
        {
            ll r = calcBest(start_id, 26, m) + calcBest(start_id, 26, ~m);
            best = max(r, best);
        }
    }
    printf("result part 2: %lld\n", best);
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
