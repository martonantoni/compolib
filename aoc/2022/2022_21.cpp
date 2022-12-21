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

struct cMonkey
{
    ll value = numeric_limits<ll>::max();
    string sleft, sright;
    int op;
};

vector<cMonkey> monkeys;

unordered_map<string, size_t> monkey_indexes;

void prepare()
{
    monkeys.reserve(2000);
    cFastFileReader in("in.txt");
    for (auto line : in)
    {
        auto ts = line.toStringVector(": ", false);
        monkey_indexes[ts[0]] = monkeys.size();
        auto& m = monkeys.emplace_back();
        if (ts.size() == 2)
        {
            m.value = stol(ts[1]);
        }
        else
        {
            m.sleft = ts[1];
            m.sright = ts[3];
            m.op = ts[2][0];
        }
    }
}

ll evalMonkey(cMonkey& m)
{
    if (m.value == numeric_limits<ll>::max())
    {
        ll left = evalMonkey(monkeys[monkey_indexes[m.sleft]]);
        ll right = evalMonkey(monkeys[monkey_indexes[m.sright]]);
        switch (m.op)
        {
        case '+':
            return left + right;
        case '*':
            return left * right;
        case '-':
            return left - right;
        case '/':
            return left / right;
        }
    }
    return m.value;
}

double evalMonkey_f(cMonkey& m)
{
    if (m.value == numeric_limits<ll>::max())
    {
        double left = evalMonkey_f(monkeys[monkey_indexes[m.sleft]]);
        double right = evalMonkey_f(monkeys[monkey_indexes[m.sright]]);
        switch (m.op)
        {
        case '+':
            return left + right;
        case '*':
            return left * right;
        case '-':
            return left - right;
        case '/':
            return left / right;
        }
    }
    return (double)m.value;
}


void Solve()
{
    printf("solution part 1: %lld\n", evalMonkey(monkeys[monkey_indexes["root"]]));
}

template<class P>
ll find_first_where_true(ll begin, ll end, const P& p)  // end is not included!
{
    for (;;)
    {
        if (begin == end)
            return begin;
        ll mid = begin + (end - begin) / 2;
        bool result = p(mid);
        if (!result)
        {
            // for 'mid' it is not true, must be after it
            begin = mid + 1;
        }
        else
        {
            // for 'mid' it is true, no use looking for start after it, but it might start at 'mid'.
            end = mid;
        }
    }
}

void Solve2()
{
    cMonkey& root = monkeys[monkey_indexes["root"]];
    cMonkey& me = monkeys[monkey_indexes["humn"]];
    cMonkey& left = monkeys[monkey_indexes[root.sleft]];
    cMonkey& right = monkeys[monkey_indexes[root.sright]];
    double rv = evalMonkey_f(right);

    ll around = find_first_where_true(0, 1'000'000'000'000'000'000ll,
        [&](ll v)
        {
            me.value = v;
            double lv = evalMonkey_f(left);
//             printf("checking %lld -> %f: %s\n", v, lv, lv < rv ? "true" : "false");
            return lv < rv;
        });

    for (ll i = around - 1000; i < around+1000; ++i)
    {
        me.value = i;
        double lv= evalMonkey_f(left);
//        printf("%lld: %f == %f     ... diff: %f\n", i, lv, rv, lv-rv);
        if (lv == rv)
        {
            printf("solution part 2: %lld\n", i);
            return;
        }
    }

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
