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

char b[100], b2[100];

#include "aoc_common.h"

enum class Hand { Rock = 1 , Paper = 2, Scissors = 3};

int Score(Hand op, Hand me)
{
    int score = (int)me;
    if (op == me)
        score += 3;
    else
    {
        if (op == Hand::Paper && me == Hand::Scissors)
            score += 6;
        else if (op == Hand::Rock && me == Hand::Paper)
            score += 6;
        else if (op == Hand::Scissors && me == Hand::Rock)
            score += 6;
    }
    return score;
}

void Solve()
{
    cFastFileReader in("in.txt");
    Hand op2h[256];
    Hand me2h[256];
    op2h['A'] = Hand::Rock;
    op2h['B'] = Hand::Paper;
    op2h['C'] = Hand::Scissors;
    me2h['X'] = Hand::Rock;
    me2h['Y'] = Hand::Paper;
    me2h['Z'] = Hand::Scissors;

    int result = 0, result2 = 0;;

    for (auto line : in)
    {
        ASSERT(line.Length >= 3);
        Hand op = op2h[line.Data[0]];
        result += Score(op, me2h[line.Data[2]]);

        Hand me = op;
        switch (line.Data[2])
        {
        case 'X':
            me = op == Hand::Paper ? Hand::Rock : (op == Hand::Scissors ? Hand::Paper : Hand::Scissors);
            break;
        case 'Y':
            break;
        case 'Z':
            me = op == Hand::Paper ? Hand::Scissors : (op == Hand::Scissors ? Hand::Rock : Hand::Paper);
            break;
        }
        result2+=Score(op, me);
    }
    printf("result part 1: %d\n", result);
    printf("result part 2: %d\n", result2);
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
 //       printf("%s\n", Solve() ? "YES" : "NO");
//        printf("result: %lld\n", result);
        //         RI(verdict);
        //         if (verdict == -1)
        //             break;
    }
}
