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

struct cElf
{
    cPosition p;
    cPosition next;
};

vector<cElf> elves;
array<int, 1024 * 1024> m;

static constexpr int m_offset = 500;

int width, height;

int& at(cPosition p)
{
    return m[p.col + m_offset + (p.row + m_offset) * 1024];
}

using cBlizzards = vector<int>;
struct 
{
    vector<cBlizzards> to_north, to_south, to_east, to_west;
} blizzards;

int mod_substract(int x, int y, int m)
{
    return (x%m-y%m+m)%m;
}

// W <----> E

bool is_cell_free(int s, cPosition p)
{
    if (p.row == -1 || p.row == height)
        return true;
    return
        none_of(ALL(blizzards.to_north[p.col]), [=](int o) { return p.row == mod_substract(o, s, height); })
        &&
        none_of(ALL(blizzards.to_south[p.col]), [=](int o) { return p.row == (o + s) % height; })
        &&
        none_of(ALL(blizzards.to_west[p.row]), [=](int o) { return p.col == mod_substract(o, s, width); })
        &&
        none_of(ALL(blizzards.to_east[p.row]), [=](int o) { return p.col == (o + s) % width; });
}

void prepare()
{
    cFastFileReader in("in.txt");
    auto firstLine = in.GetNextLine();
    width = firstLine.Length - 2;
    blizzards.to_north.resize(width);
    blizzards.to_south.resize(width);
    cPosition p(0, 0);
    int r = 0;
    for (auto line : in)
    {
        if (line.Data[1] == '#')
            break;
        blizzards.to_east.emplace_back();
        blizzards.to_west.emplace_back();
        for (int c = 0; c < width; ++c)
        {
            switch (line.Data[c + 1])
            {
            case '<':
                blizzards.to_west[r].emplace_back(c);
                break;
            case '>':
                blizzards.to_east[r].emplace_back(c);
                break;
            case '^':
                blizzards.to_north[c].emplace_back(r);
                break;
            case 'v':
                blizzards.to_south[c].emplace_back(r);
                break;
            }
        }
        ++r;
    }
    height = r;
}

using cState = pair<int, cPosition>; // s, r, c

void Solve()
{
    deque<cState> states;
    vector<char> visited(width*(height+2), 0);
    int last_step = 1, stage = 0;
    states.emplace_back(0, cPosition { -1, 0 });
    for (;;)
    {
        auto [step, p] = states.front();
        states.pop_front();
        if (step != last_step)
        {
            fill(ALL(visited), 0);
            last_step = step;
        }
        char& v = visited[(p.row + 1) * width + p.col];
        if (v)
            continue;
        v = 1;
        for (auto o : neighbour4Positions)
        {
            auto checked_p = p + o;
            if (checked_p.col == width - 1 && checked_p.row == height && (stage == 0 || stage == 2))
            {
                printf("Got out in %d steps.\n", step + 1);
                if (stage == 2)
                    return;
                ++stage;
                printf("going back...\n");
                states.clear();
                states.emplace_back(step + 1, checked_p);
                goto again;
            }
            if (checked_p.col == 0 && checked_p.row == -1 && stage == 1)
            {
                printf("got back to start in %d steps.\n", step + 1);
                ++stage;
                states.clear();
                states.emplace_back(step + 1, checked_p);
                goto again;
            }
            if (checked_p.isWithinBounds(width, height) && is_cell_free(step + 1, checked_p))
            {
                states.emplace_back(step + 1, checked_p);
            }
        }
        if (is_cell_free(step + 1, p))
            states.emplace_back(step + 1, p);
    again:
         int alma = 3;
    }
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