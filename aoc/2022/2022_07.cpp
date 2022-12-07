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
#include <cstddef>
#include <cinttypes>
#include <regex>
#include <string>
#include <type_traits>

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

ll round_up_div(ll osztando, ll oszto)
{
    return (osztando + oszto - 1) / oszto;
}
ll gcd(ll a, ll b)
{
    while (b) b ^= a ^= b ^= a %= b;
    return a;
}

#include "aoc_common.h"

char buf[10000];

struct cDir
{
    map<string, cDir*> dirs;
    cDir* parent_dir = nullptr;
    ll direct_size = 0;
    vector<pair<string, ll>> files;
    void addFile(string f, ll size)
    {
        direct_size += size;
        files.emplace_back(f, size);
    }
};

cDir* root;

ll result = 0;

ll dfs(cDir* d)
{
    ll size = d->direct_size;
    for (auto& [name, sub_dir] : d->dirs)
    {
        size += dfs(sub_dir);
    }
    if (size <= 100000)
    {
        result += size;
    }
    return size;
}


void Solve()
{
    cFastFileReader in("in.txt");

    root = new cDir;
    root->parent_dir = root;
    cDir* pos = root;

    for (auto line : in)
    {
        auto tokens = line.toStringVector(" ");
        if (tokens[0] == "$")
        {
            if (tokens[1] == "cd")
            {
                auto folder = tokens[2];
                if (folder == "/")
                {
                    pos = root;
                }
                else if(folder=="..")
                {
                    pos = pos->parent_dir;
                }
                else
                {
                    auto& sub_dir = pos->dirs[folder];
                    if (!sub_dir)
                    {
                        sub_dir = new cDir;
                        sub_dir->parent_dir = pos;
                    }
                    pos = sub_dir;
                }
            }
        }
        else
        {
            if (tokens[0] == "dir")
            {
                auto folder = tokens[1];
                auto& sub_dir = pos->dirs[folder];
                if (!sub_dir)
                {
                    sub_dir = new cDir;
                    sub_dir->parent_dir = pos;
                }
            }
            else
            {
                pos->addFile(tokens[1], atoll(tokens[0].c_str()));
            }
        }
    }

    dfs(root);
    printf("result part 1: %lld\n", result);
}

ll needed = 30000000;
ll result2 = numeric_limits<ll>::max();

ll dfs2(cDir* d)
{
    ll size = d->direct_size;
    for (auto& [name, sub_dir] : d->dirs)
    {
        size += dfs2(sub_dir);
    }
    if (size >= needed)
    {
        result2 = min(size, result2);
    }
    return size;
}

void Solve2()
{
    ll total_size = dfs(root);
    ll already_free = 70000000 - total_size;
    needed = 30000000 - already_free;

    dfs2(root);
    printf("result part 2: %lld\n", result2);
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
