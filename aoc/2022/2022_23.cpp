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

int& at(cPosition p)
{
    return m[p.col + m_offset + (p.row + m_offset) * 1024];
}

bool is_elf(cPosition p)
{
    return at(p) != -1;
}

pair<cPosition, cPosition> boundingBox()
{
    cPosition top_left = elves[0].p, bottom_right = elves[0].p;
    for (int i = 1; i < elves.size(); ++i)
    {
        top_left.row = min(elves[i].p.row, top_left.row);
        top_left.col = min(elves[i].p.col, top_left.col);
        bottom_right.row = max(elves[i].p.row, bottom_right.row);
        bottom_right.col = max(elves[i].p.col, bottom_right.col);
    }
    return { top_left, bottom_right };
}

void drawMap()
{
    auto [top_left, bottom_right] = boundingBox();
    int h = bottom_right.row - top_left.row + 1, w = bottom_right.col - top_left.col + 1;
    vector<char> b(w * h, '.');
    for (auto& elf : elves)
        b[(elf.p.row - top_left.row) * w + elf.p.col - top_left.col] = '#';
    for (int r = 0; r < h; ++r)
    {
        printf("%.*s\n", w, b.data() + r * w);
    }
}

void printElfPositions()
{
    auto [top_left, bottom_right] = boundingBox();
    for (auto& elf : elves)
    {
        printf("%d, %d\n", (elf.p.row - top_left.row) + 1, (elf.p.col - top_left.col) + 1);
    }
}

void prepare()
{
    fill(ALL(m), -1);
    cFastFileReader in("in.txt");
    cPosition p(0, 0);
    for (auto line : in)
    {
        for (p.col=0; p.col < line.Length; ++p.col)
        {
            if (line.Data[p.col] == '#')
            {
                at(p) = (int)elves.size();
                elves.emplace_back().p = p;
            }
        }
        ++p.row;
    }
}

using Direction = array<cPosition, 3>;
array<Direction, 4> dirs =
{ {
    {{ direction_N, direction_NE, direction_NW }},
    {{ direction_S, direction_SE, direction_SW }},
    {{ direction_W, direction_NW, direction_SW }},
    {{ direction_E, direction_NE, direction_SE }},
} };

void Solve()
{
    unordered_map<cPosition, int> proposals;
    vector<int> active_elves;
    for (int round = 1;; ++round)
    {
//         drawMap();
        proposals.clear();
        active_elves.clear();
        for(int i=0;i<elves.size();++i)
        {
            if (any_of(ALL(neighbour8Positions), [&](cPosition offset) { return is_elf(elves[i].p + offset); }))
            {
                for (int j = 0; j < 4; ++j)
                {
                    auto& ds = dirs[(j + round - 1) % 4];
                    if (none_of(ALL(ds), [&](cPosition offset) { return is_elf(elves[i].p + offset); }))
                    {
                        ++proposals[elves[i].next = elves[i].p + ds[0]];
                        active_elves.emplace_back(i);
                        break;
                    }
                }
            }
        }
        bool moved = false;
        for (int i : active_elves)
        {
            auto& elf = elves[i];
            if (proposals[elf.next] == 1)
            {
                if(at(elf.p)==i) 
                    at(elf.p) = -1;
                at(elf.next) = i;
                elf.p = elf.next;
                moved = true;
            }
        }
        if (round == 10)
        {
            auto [top_left, bottom_right] = boundingBox();
            int area = (bottom_right.row - top_left.row + 1) * (bottom_right.col - top_left.col + 1);
            printf("after round %d bounding box area: %d, elves: %d, empty: %d\n", round, area, (int)elves.size(), area - (int)elves.size());
        }
        if (!moved) // active_elves.empty())
        {
            // 1117 too high
            printf("\n\nno elves wanted to move after %d steps.\n", round);
            auto [top_left, bottom_right] = boundingBox();
            int area = (bottom_right.row - top_left.row + 1) * (bottom_right.col - top_left.col + 1);
            printf("bounding box area: %d, elves: %d, empty: %d\n", area, (int)elves.size(), area - (int)elves.size());
            break;
        }
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