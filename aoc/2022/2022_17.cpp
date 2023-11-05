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


char wind[] =
">>>><<<>><<><<><<><>>>><>><><<<<>>>><<<>><<<>><<<>>><<<>><<<<>>><<<<>>><<<>><><<<<>>>><<<<>>>><<>>><>>>><<>>><<<<>>><><<<<><>>>><>>>><>>>><><>><>>><<>>>><>>>><<<>>>><<>>><<<<>>><><<<<>>><>>><<<>>>><>><><<<>>><<<>>>><<>><<><<<>>>><><<>>><<><<<><<<><><><<<<>>>><<<<>>><<<<>><<>>><><<<<>><<<<>>><<>>>><<><>><<<<>>>><<<>>>><<<>>>><><<<>>>><<<<><<><<>>>><<><<<<>><<>>><<<><<>><><<<<>><<<><<<>><<<>><<<<><>>><<<<>>><<<>>><<<><<<>>>><<>>><<<>>>><<<>>><><<<<>><<<>>><<<>><<<><<<>><<>>>><<>><<>>>><<><<<>>><<<<>>>><<<>><<>>>><>><>><>><<<<>><><>><<<>><><>>>><<><<<<>><>>>><<<<>>><<>><<<>><<<>>><<<<><<<><<<<>>>><<<<>>>><<<><<<<>><<<>>>><>>>><<<>>>><<<<><><<<<>><>>>><<<>><><<<<><<>>><>>>><>>><<<<><<><<<><>><<<<>>><>>><>>>><<<><<<><>><<<>>>><<<<>>><<><<>>>><><>>><><<>>>><<<>>>><<<>><><<<<><<<>>><<>>><<<>><<<>><<>>>><<>>><<<><<<>><<<<><>><><<<>>>><<<<>>>><<<<>><<<<>>>><<<<>><<>>>><<<<><<>>><<>>><<<<>>><<<>>><>>><<<>>>><<<<><<>>>><<<<>>>><<<<>>>><>><<>>>><><<<>>><>>><<<>><<<<>>><>>>><<>>>><<<>>>><<<<>><<<>>>><>>>><<<<>>>><<<<>><<>>><<><>>><<<<><<<<><<><<<<>>><>>>><<<<>>><><>><<<<><<<<>><<>>>><>>><<>>><<>>>><<>><><>>>><>><<<<>>><<>>>><<<<>>><<<>>>><<<>>><<<<><<><<<>><><<<<>>><<<><<<>><<<<>>><>>><>>><<<<>>>><<<>>>><<>>><<<<>>><<<<><<<<>>>><<>><<<<>>><<<<>>>><>>>><<<<>><<<<>><<>>><><<><<><<<<>><<>>>><<>><<<>>>><>>>><>>>><<<><<<>>>><<><<>>><<<>><<<<><<<<>><<<<>><<<<>>><<>><>><<>><<<>>>><<<>><>>>><<<>>>><<>><<>>>><>><<<><<<>>>><<<><<>>><<<>>>><<><<<>>>><<<<><<<>>>><<<>>><<>><<><<>>>><<><<<>><<<<>><<>><>>>><>><<<<>>><>>>><<>><<>>><<<<>>>><<<<>><>><<<>>>><><>>><<><<<<><>><<<>>><<<>><><><<<>><<<>>>><<<<>>><>>>><<<><>><<<>><<>>><<<><<<<>><<<>>><><<>><>>><<<>>>><>>><<>><<<>>>><<<>>><><<<<>>><>>>><<<<>>><<<<>>><<<><><><<<>>><><<>>>><<<>>>><>>>><><<<>>>><<<>>>><>>><<>>>><<<>>>><<>><<<<>>><<<>>>><<<><>><<<<>>><>><<<><<<>>>><>>><>>><<>><<<>><<<<><<<>><>>><>><<<>>><<<>><<<<>>><<<><<>>><<<<>>><<<><>>>><<><<>><>>><<>><<>>><<<><<<><<<><<<>><<>>>><>>>><>><<<<>>><<<<><<<<>><>>>><<<>>>><<<>><<<>>><<<<>>><><>><<<<><<><>>>><<<>><><<><<>>>><<>><><<>><<<>><<<<>>>><><<<<>><<<><<>><><<>><>>><>>><<<<><<>><<>><<><<>>>><><<<><<<>>>><<<>>>><<><<<<><>><<<<>>><<<<>><>>>><>><>>><<<>>><<<>>><<<<>><>><<<<>>><<<>>>><>>>><<<><<<>><<>>><<>>><<>><>><<<<>>>><>>><>><<>>>><<<<>><>><<<<>>>><<>><<<<>><>>><<><<<>><<<>>><<<<>>><<><<>>>><<<>>><<<<>>><<<<>><><<>><<>><<<<>>>><<<><<>>>><<<><><>>><><<<>>><><<<<><<<<><<<>><<<>>>><>>><<<<>><<<<><<<>>><<><<<>>>><<<>>><<<<>><<<<>>><<<<><>>>><<<<>><<<><<<<>>>><>><<<<>><<<<>><<<><<<>><<<>>>><>>><<<>>><<<<>><<>>><<<>>>><>>>><<>>><<<><<>>><<<<>>>><<>>>><<<>><<<<>>>><><<<>>><<<>>>><<<>>><<<><<><<<<><><<<<>>>><<<>>>><><><<<<>><<>>>><<<<>>>><>>>><<<<>>>><><<><<>><<><<>><<<<>>><<>>><<<<>>>><<<<>>>><<<><>><<<<>>>><<<><>>>><>>><<><><><<<>>><<>>>><<>>><<>><<<<>>>><<<>>>><<>>><<><>>>><<<>><>>>><<>><>>><>>><<>><><<>>>><<>>>><>><<<<>><<<<>>>><>>>><>>>><>>><>>>><<<>><<>>>><<<>>>><><<<<><<>>><<<>>>><<>>><<><><<<<>>><<<<>>>><<<<>>><<>><<<>><<>><<<<>><<<>>>><<<>>>><<<<>>>><<>>>><<<<><<<>><<<>>><<>>>><>>>><<<<>><<<>><<>><<<>><<>>><<<<><<<<>>><<>><<<<><<<<>>>><><<<<>>><<<><>>><<>><<<<>>><<<<>>>><>><<<<>>><<<<>>>><<<>>>><>>><<><><>>><<<<>>><<<<>>><<<<>><<<<>>><<<<>>>><<<<><<<>>>><>>>><<<<>>><<<>>>><>><<<<><<>>>><>>><<<<><<<<>>>><><<<><<<>>>><<<><>>>><<<<><<<><>>>><<<>><<<>><<<><<<<>><<<<>><<><<>>>><<>>>><<<<>>><>><<<<><<<><<>>>><<<>><>>>><<<>>>><<>><>>><><<><<<>><<>><<<>><<<<><<<<>>><>><<<>><><<<<><<>>><>><<<<><<<>>>><<<><<<<><<>>>><><<<>><<>><<<><<<<>>>><<>><<>><<><<>><>><<<>><<>><<<>><>>><<<<><<<>>>><<<<>><>><<><><>>><>>>><<<<>>>><<<>>>><<<>>><<<<><<<>>>><<><<<<><<<><><<>>><<>>>><<>><<<<>>><<<>>>><<<><<<>>>><<<>>><<<>><<<>><<<<>><<<><>>>><>>>><<<<>>>><<>>><<><>><<<<>>>><<<>>>><>><>>><<><<<><<<><><<<>>><<<<><<<<>>>><<<>>>><<<<>>>><<<><<<<><<<<>><<<>>>><<>>>><>>><<>><<<<>>>><<>><>><<>><<<>><<<<>>><<<<><<<<><<>><<<>><<><<<<>><<><<<>><<<<><<<>>><<<>>><>>>><<<><<<<>><>>><<<>><<>>><<<<>><><<>>><<><<>>><<>>><<<<>><<<<>>>><<<>>>><>>>><<>>>><<<>>>><>>><<<<>><>>>><<<<>>>><>><<<>><>>><<<<><>><><<<>><><<>><<<<>>><<<<><<>>>><<>>><<<<>>>><<<<><<<<><<<>><>>>><<<>>>><<>>>><<>><><>>><<<>>><<>>>><><<<<><<>>><<>>>><<<>>><<<<>><>>><<<<>><>>><><>>><<>>><><>>>><<<>>>><<<>><<<<>>>><<<<>>>><<>><<<<><<<<>>>><<<<>>><<<>>><<<<><<<<>><<<<>>>><<<>>><<>><<><>><<>>><>>><<>>><<<<>>><<<>>>><<<><><<>><<<><<<>>><><>><<<<><<>><<<<>><<<<><<<<>><>>>><<<>>>><<<<><<<><<<<>>><<>>>><>>><<<><<>>>><>>>><>>><<<<>>>><<<>><>>><<><<<<>><<>><<>>><<<<><<>>>><<<>><<<<><<>>><>>><<>><><<<<>><<<><><<<>>>><<<>><>><<>><<<>>><<>>><<<>>>><<<>>><><<>><>>>><>>><>><<<<><><<<>>><>>>><><<<<><<<><>>><<<>><<<<>>>><<<<>>><<>><<<<>><<>><<>>><<<>>>><<>>>><<><<<>>><<<<>><<<>><<<>>><<<<>>>><><<<<>><<>><<<<>><<><<<<>>>><<>>><<<><>>>><<>>>><<>><<<><<>><<<>>>><<<><<><<<<>><<<<>>>><<<>>>><<<<>>>><<<>><><<<>>>><<<<>>><<>>><<<>><><<<<>>><>>>><<<<>>>><<<<>><<<><><<<<><<<>>>><<<<>>>><><<<>><>>>><<<<>>>><<<<><<<><<<<>>><>>>><>>>><<<<>><<>><<<>>>><<<<>>>><<<<><<<<>>>><<>>><<>>><<<>>>><<>>>><<<>>>><<>><<>>>><<<<>>>><<<<><<<><>><<>><<<<><>>>><>><<<>>>><<<><<>>>><<><><><>>>><<<<>>><<><<<>><<<<>><<<>><<<>>><<<<>>>><<<>>><<<<>><<<>>>><<<>><<<>>>><<<<>>>><<>>>><<<<>><<<>>><<>>><<<<>>>><<<<>><<<<>><<<<>>>><<<><<<>><>>><>>>><<>><>>>><<>><<<>>>><>>>><>><>>><<<<>>><<<<>>><<<<>>><<<>>>><<<>>>><<<<>>><<<>>><<>>>><>>><<>>><>><><<<>>><><<<>>><<<>><<>>><<<<>>><>>><<><<>>><<<<>>>><<>>><<<>><<>><<<>>><<<<>>>><<<>>><<<<>>><<<<>>><<<><<>>>><<<<>>><<<>>>><<>>><<>><>>>><<<>>>><<<<>>>><>>>><>>>><<><<><<<>><<<>><<><<<<>>><<<<>><>>>><<>>><<>>>><<<>><<<><<<>>>><<<>>><<<<>>><<<>>><<<<><<><<<<>><<<<><<><<<>><<<<>><><<<<>>>><<<<>><<>><<>>>><><<>><>>><<>><<<><><<>><<<<>><>><<<<>>><<><<>><>>>><>>>><<<>>><<<><<>><<><<<>>>><<<><<>>>><>>>><<<>>>><<>><<<>><>><><<<>>>><<>><<<<>><<<<>>><<<<>>>><<<<>><<<<>>><>>><>>><<><>>><<>><<<<>>>><><<<>><<<<>>>><>><<<>><<><><<><<<<>><<>>>><>>>><<>>><<>><<><><<>>><>><<<<>>><<<><<<<><><<>>>><<>>>><<<<>>><<<>>><<<>>>><<<>>><<<<>>>><<>>>><<>><>><<<>>><<<<><<<>>><<>>><<<<>><><>>>><<<>><<>>><<<><<><<<<>><<<<>>>><><<<>><<<<><<><<><<<>>>><>>><>><<<>><<<>>><><<<><<<<><>>>><><<>><<<>><<>>><<<>>>><<<<><>>>><<<>>><><<<><<<>><<>><<>><<<<><<<<>>>><><<<>>><<>><<<>>>><<>>><<<<><<<<>><<><<<>><<<<>>><<<<><<>>>><<<<>><<<>>>><><<>><>><<<><<>>>><<>>>><<<>>>><<<<>>>><<>>><><<<<>>>><<>>><><<<>><>>><<<<>>>><<<<><<<>>><<>>>><><<>><<>>><<<>>>><>><><<<<>><<<>>>><<>><<<>><<><<<<>><><<><<<>>>><<><<<<>>><<><<<>>>><>><<<>>>><<>>><><<<<>><<<<>>>><<>>>><><>>><<<><<>>>><<>>><<<>>><<<><>>>><<>><<<>><><<<>>>><>><>>><<<<>><<>>>><><<<>>>><<>>>><<<<><>>>><<<>>>><<<>><<<>>><<<<><<>>>><<<<>>><<>>><><<<<>>><>>>><<><<<>>>><<><<><<<<><<<><<>>>><>>><><<<<>>>><<<<>>>><><<<<>>><<>>>><<<<>>>><<<>>>><<>>><<<<>><<>>>><<><<><<>><<<<>>><<><<<<><>>>><<>><<>>>><<><>><<<>><<>>>><<<>>><<<<><<<<>>>><><<<<><<<><<>>><<<><<<<>>>><>>>><<<><<>><>>>><<<<><>>><<<>>><<><<<<>>>><<<<>><<<<>>>><<<<>><<<<><>><<<>>><>><<<<>>>><<<<><<<>>><<><<<<>>>><<>>>><<<<>>>><<>>>><<<<>>>><<<>>>><><>><<<>>>><<<<>>><<<<>>>><<<>>><<<<>>>><<><<>><><<>>>><>>>><<<<>>>><<>>>><>>><<<>><>>>><<<<>>><>>>><<<<>>>><<><>><<><<<<>>><<<<>><<<<>>>><>>>><<<>>><<>>>><<<<>>>><<>>><<<<><<>>>><>>><>><<<<>><<<<>>>><<<<>>><<><<>>><<><><<<>>>><<<>>><<<>><<<<><<<<>>><<>><<<<>>>><<<<>>>><<<><<<><><<>>><<<<><<<>><<<>>><<<<><><<>><<<>>>><<<<><<>>>><<<<><<>>><<>>>><<<<><>><<<<><<<<><<>>>><<><<<>><>><<<>><>>><<<>><>>>><<<>><<>>>><<><<>>><<>><>>><<<<>>><>><<<><<><><><<>>><<<><>><<>><<>><<>>><<<<>>>><<<>>><<<>>>><<<<>>><>><<>>><>>>><<<>><<<>><<<<>>><<<<><<>>>><<<<><>>>><<<<>>><<<>>>><<<>>>><<<<>><<>>>><>>><><<<<>>><<<<>><>>><<<<><<<<>>><><<<>>>><<<<>><<<>>>><<<<>>><<>><<>><<<<>><<>><<<<>><<>>>><>>>><<<<>>>><<<<>>><><>>><<>>><<<<>>>><>>>><>><<<<>>>><<><<<><<>>>><<<><<<><<>><<<<>>>><<<><<>><<<>><<>><<>>><<><<<>>><<<<>>><<<>>><>>>><<<<><<>>><>>><<<>>><>>>><<<<>>>><<<>>><<<<>><<<><<<<><<<>>><><<<<>>><<<>>>><<>>>><<>><<<>>>><<<<>>><<<><<<>><<<<>>><<<>>>><<<>>><<<>><<<>>>><<>><<<<>><<><><<>>><>>>><<<<>>>><<>>>><<<>>><<<>><<<<>>><<<<><<<<>>><<<<>>><><<>>>><<<<>>>><><<<<>><<>><<><<><<<>>><<<<><<>>>><<><<><>><<<<>>>><>><>><<>>>><<<>><<<>>>><<<>><>><<>><<<>>><<<<><<<>>><<<>>><<>>>><>><<<<>><<<>><><<<<>>>><>>>><>>>><<<>>><>>>><<<>>>><<<>>>><>><<<<>><><>>><><<>>>><<<>>>><<>>><<<>><<<<>>><<>>><>>><<<<>>>><><<<<>>><<>>><><<<>><<<<><<<><>>>><<>><<>><<><<<<><<<><<>>><<<>>>><<>><<<<>>><<<<>><><<<<>>>><<<<>>><><<<>><<>>>><>><>><<<>>>><<>><>>><<>>><<><<>>>><>>><><<>>><<>><<>>>><<<><<<<><>>><<<<>>>><<<>>><<<<><<<>>><<<<><<>><<<<><<<<>>>><>>><>><>>><>><>><><<<>>><><>>>><<<>>>><>><<>>>><<<>><<<>>><<>>>><>>>><>>><<<<>>><<>>><<<>><<<<>><<<>>>><<<>><<<<>><>>><<>>><>>><<<>>>><<>><<<<><<<><>>><><<<>>><<>><><<<>>>><>>>><<<<>>>><<<><<<><>>><<<>>>><<<<><<<<><<>>><<>><<<>><>>><<>>><<>>>><<<<><<<<>><<<>><<>>>><><<<<><<<<>>><<<>>>><<<<>><<>>>><>>>><>><<<>>><<<>><<<><<<>>>><><<<<>><<>>>><>>><<<>>>><<><<<<>><<<>>>><<<>>><<<>><<<>>><><<<<>><<<<><<<<>>>><<<>>>><>>><<><>>><<<>><>><>>><<>>>><<<>>><<>>>><>>>><<<><<<>>><<<<><<<>><<<>><<<><<>>><<<>><<<<>>><<<<>><<>><<<<>>>><<>><<<>><>>><<<<>><>>>><<<<><<<<>><>>>><<<<><<><<<><<>><><<<>>>><<<<>>><<><>>>><<>><<<<>><<<<>><<<><<>>><<>>>><<<<>>>><<><<<<>>>><<><<<><<<><<<<>>><<<>>><<<>><>><><<<<>>>><>><<>>>><<><><<>>><<<<>><<<><<<>>>><<<><<<<>><<<<>>>><<<>>><<<>>><<>>><><<<>><<<<>>><<>>><<<<>>><<>>><>>>><<<>>><<<<>>><<<>><<<<>><<<>><<><<<<>>>><<>><<<>>>><<<>>><<<<>>><<<<>>><<<<><<<<>>>><<<<>>><<><<<><<<<>>><<<<>>>><<<>>><<<<>>>><<><<><<<<>><<<<>>><>>>><>>><<>>>><>><<>>>><<<>><<<>>>><><<<>>>><>>><<<<>><><<<>>><<>>><<<<>><>><<>>>><<<<>><<>><>>><<<<>>>><<<>><>><<<<>>>><<<>><<>>><<<>><<<>><<><>>>><><<<>>><<<<>>><<<<><<<<>>><>>><<<<><<<>>><<>><<<<>>>><<<<>>><<<<><><<<<>>><<>><>>><<<>><<><<<<>>><<><<<><>><<<>><<>>><<<>><<<<><<<><<<>>>><<<<>><>><<<<>><<><<<>><<<>>>><<<<>>><>>><<>>><>>>><<<<>>><<<<>>><<>><<>>>><>><>>>><<>><<<>><<<>>>><<<<>>><<>>>><><>><><<<><<>>><<>>><<<<><>>><<<<>><<<<>>><>><<<<>><<<<>>><>>>><><<<<><>>><>>><<<>>><>>><<<<><>><<<><<<<>><<>><><<>>>><<>><<<<><>><<>>>><<>><<>>><<>>>><<<<><>>>><<<>>><<><<>><<<>>><<<<>>><<>><><<<<>>>><<<<><>>>><<<<><<<<><<<>><<<>>>><<<>>>><<<>>>><<<>><<<<>>>><<<<><<<<>>>><<>><<>><<<>><<<<>><<<>><";

//char wind[] = ">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>";

struct cShape
{
    int h;
    int bits[4];
};

cShape shapes[5] =
{
    {
        1,
        { 
            0b000000'000,
            0b000000'000,
            0b000000'000,
            0b111100'000,
        }
    },
    {
        3,
        {
            0b000000'000,
            0b010000'000,
            0b111000'000,
            0b010000'000,
        }
    },
    {
        3,
        {
            0b000000'000,
            0b001000'000,
            0b001000'000,
            0b111000'000,
        }
    },
    {
        4,
        {
            0b100000'000,
            0b100000'000,
            0b100000'000,
            0b100000'000,
        }
    },
    {
        2,
        {
            0b000000'000,
            0b000000'000,
            0b110000'000,
            0b110000'000,
        }
    }
};

void prepare()
{
//     cFastFileReader in("in.txt");
//     for (auto line : in)
//     {
//     }
}

struct cPit
{
    int pit[1024];
    ll top;
    cPit()
    {
        for (auto& v : pit)
            v = 0b1'0000'000'1;
        top = 0;
        pit[0] = 0b1'1111'111'1;
    }
    int& at(ll y)
    {
        return pit[y & 1023];
    }
    bool check(const cShape& s, ll bottom_y, int x)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (at(bottom_y + 3 - i) & (s.bits[i] >> x))
            {
                return false;
            }
        }
        return true;
    }
    void rest(const cShape& s, ll bottom_y, int x)
    {
        for (int i = 0; i < 4; ++i)
        {
            at(bottom_y + 3 - i) |= s.bits[i] >> x;
        }
        ll new_top = max(top, bottom_y + s.h - 1);
        while (top < new_top)
        {
            ++top;
            at(top + 7) = 0b1'0000'000'1;
        }
    }
    void print()
    {
        for (int i = 20; i >= 1; --i)
        {
            printf("|");
            for (int x = 7; x >= 1; --x)
                printf(at(i) & (1 << x) ? "@" : ".");
            printf("|\n");
        }
        printf("+-------+\n\n");
    }
};



void Solve(ll r)
{
    cPit pit;
//     pit.print();
    char* wp = wind;
    ll prev_h = 0;
    ll prev_i = 0;
    ll h_at_mark;
    ll height_adjust = 0;
    for (ll i = 0; i < r; ++i)
    {
        ll rock_i = i % 5;
        int x = 3;
        ll bottom_y = pit.top + 4;
        for (;;)
        {
            if (*wp == '<')
            {
//                 printf("wind <<< : ");
                if (pit.check(shapes[rock_i], bottom_y, x - 1))
                {
//                     printf("success\n");
                    --x;
                }
                else
                {
//                     printf("failed\n");
                }
            }
            else
            {
//                 printf("wind >>> : ");
                if (pit.check(shapes[rock_i], bottom_y, x + 1))
                {
//                     printf("success\n");
                    ++x;
                }
                else
                {
//                     printf("failed\n");
                }
            }
            if (!*++wp)
            {
                wp = wind;
                printf("wind reset. rock: %lld, height: %lld, diff: %lld\n", rock_i, pit.top, pit.top - prev_h);
                printf("i: %lld, prev_i: %lld, diff: %lld\n", i, prev_i, i - prev_i);
                if (pit.top> 16176)
                {
                    h_at_mark = pit.top;
                    ll remaining = r - i;
                    ll skipped_cycle = remaining / (i-prev_i);
                    ll rest = remaining % (i - prev_i);
                    height_adjust = skipped_cycle * 2695;
                    printf("remaining: %lld, skipped_cycle: %lld, rest: %lld\n", remaining, skipped_cycle, rest);
                    i += skipped_cycle * 1735;
                }
                fflush(stdout);
                prev_h = pit.top;
                prev_i = i;
            }
            if (!pit.check(shapes[rock_i], bottom_y - 1, x))
            {
                pit.rest(shapes[rock_i], bottom_y, x);
                break;
            }
            --bottom_y;
        }
//         if (i < 4)
//         {
//             printf("\n\nafter rock %d:\n", i+1);
//             pit.print();
//         }
    }
    printf("Height after %lld rock: %lld\n", r, pit.top + height_adjust);
}

void Solve()
{
    Solve(2022);
}

void Solve2()
{
    Solve(1'000'000'000'000ll);
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