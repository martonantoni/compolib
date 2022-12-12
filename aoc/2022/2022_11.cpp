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

#include "aoc_common.h"

struct cMonkey
{
    vector<ll> items;
    ll test;
    int targets[2];
};

cMonkey ms[8];

struct cPosition
{
    ll worry;
    int monkey;
    int round;

    cPosition() {}
    cPosition(ll worry, int monkey, int round) : worry(worry), monkey(monkey), round(round) {}
    bool operator==(const cPosition& p) const 
    {
        return worry == p.worry && monkey == p.monkey;
    }
    bool operator!=(const cPosition& p) const 
    {
        return !(*this == p);
    }
};

struct cLoopData
{
    vector<cPosition> steps;  // pos, round
    ll length;
    ll before_loop_length;
    int loop_activity[8] = { 0 };
    int rounds;
};

struct cItem
{
    ll x;
    int start_pos;
    cLoopData loop;
};

vector<cItem> items;

ll lkkt = 3;

//#define USE_EXAMPLE

#ifdef USE_EXAMPLE

ll calcWorry(int monkey, ll worry)
{
    switch (monkey)
    {
    case 0:
        worry *= 19;
        break;
    case 1:
        worry += 6;
        break;
    case 2:
        worry *= worry;
        break;
    case 3:
        worry += 3;
        break;
    }
    worry /= 3;
    return worry % lkkt;
}

constexpr static int M = 4;

#else

ll calcWorry(int monkey, ll worry)
{
    switch (monkey)
    {
    case 0:
        worry *= 5;
        break;
    case 1:
        worry *= worry;
        break;
    case 2:
        worry += 1;
        break;
    case 3:
        worry += 6;
        break;
    case 4:
        worry *= 17;
        break;
    case 5:
        worry += 8;
        break;
    case 6:
        worry += 7;
        break;
    case 7:
        worry += 5;
        break;
    }
//    worry /= 3;
    return worry % lkkt;
}

constexpr static int M = 8;

#endif


void prepare()
{
    cFastFileReader in("in.txt");
    unordered_set<ll> is;
    for (int i = 0; i < M; ++i)
    {
        in.GetNextLine();
        auto l = in.GetNextLine();
        auto xs = l.toStringVector(" ,", false).ToIntVector();
        for (auto x : xs)
            if (x != 0)
            {
                ms[i].items.emplace_back(x);
                items.emplace_back();
                items.back().x = x;
                items.back().start_pos = i;
            }
        in.GetNextLine();
        ms[i].test = stol(in.GetNextLine().toStringVector(" ", false)[3]);
        lkkt *= ms[i].test;
        ms[i].targets[0] = stoi(in.GetNextLine().toStringVector(" ", false)[5]);
        ms[i].targets[1] = stoi(in.GetNextLine().toStringVector(" ", false)[5]);
    }
    for (auto& item : items)
    {
        item.loop.steps.emplace_back();
        int monkey = item.loop.steps.back().monkey = item.start_pos;
        int round = item.loop.steps.back().round = 1;
        ll worry = item.loop.steps.back().worry = item.x;
        for (;;)
        {
            ll old_worry = worry;
            worry = calcWorry(monkey, worry);
            int next_monkey = worry % ms[monkey].test == 0 ? ms[monkey].targets[0] : ms[monkey].targets[1];
//             printf("monkey %d -> %d, worry: %lld -> %lld. round: %d\n", monkey, next_monkey, old_worry, worry, round);
//             fflush(stdout);
            if ( next_monkey <= monkey )
            {
                ++round;
            }
            cPosition new_pos(worry, next_monkey, round);
            auto i = find(ALL(item.loop.steps), new_pos);
            if (i == item.loop.steps.end() || next_monkey > monkey || i->round == (i-1)->round)
            {
                item.loop.steps.emplace_back(new_pos);
            }
            else
            {
                item.loop.length = item.loop.steps.size() - (i - item.loop.steps.begin());
                item.loop.before_loop_length = item.loop.steps.size() - item.loop.length;
                item.loop.rounds = round - i->round;
                printf("loop established. Item start worry: %lld, loop length: %lld, before_loop_length: %lld, rounds: %d\n", 
                    item.x, item.loop.length, item.loop.before_loop_length, item.loop.rounds);
                break;
            }            
            monkey = next_monkey;
        }
        for (auto i = item.loop.steps.begin() + item.loop.before_loop_length; i != item.loop.steps.end(); ++i)
        {
            ++item.loop.loop_activity[i->monkey];
        }
    }
}

void Solve(ll r)
{
    vector<pair<int, ll>> activity(M); // monkey #, activity
    for (int i = 0; i < M; ++i)
    {
        activity[i].first = i;
        activity[i].second = 0;
    }
    for (auto& item : items)
    {
        auto& loop = item.loop;
        auto& steps = item.loop.steps;
        bool oops = false;
        for (int s = 0; s < loop.before_loop_length; ++s)
        {
            if (steps[s].round > r)
            {
                oops = true;
                break;
            }
            ++activity[steps[s].monkey].second;
        }
        if (oops)
            continue;;
        ll round = steps[loop.before_loop_length - 1].round;
        ll remaining_rounds = r - round;
        ll full_loops = remaining_rounds / loop.rounds;
        for (int m = 0; m < M; ++m)
        {
            activity[m].second += loop.loop_activity[m] * full_loops;
        }
        remaining_rounds = remaining_rounds % loop.rounds;
        auto first_step_in_loop = loop.steps.begin() + loop.before_loop_length;
        ll end_round = first_step_in_loop->round + remaining_rounds;
        for (auto i = first_step_in_loop; i->round != end_round; ++i)
        {
            ++activity[i->monkey].second;
        }
    }

    sort(ALL(activity), [](const auto& l, const auto& r) { return l.second > r.second; });
    printf("result for %lld rounds: %lld * %lld = %llu\n", r, activity[0].second, activity[1].second, ull(activity[0].second) * ull(activity[1].second));
}

void Solve()
{
    Solve(10'000);
}

void Solve2()
{
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
