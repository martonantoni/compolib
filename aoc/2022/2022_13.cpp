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

struct cPacket
{
    vector<variant<int, cPacket>> items;
    bool isDivider = false;

    int compare(cPacket& r)
    {
        for (int i = 0;; ++i)
        {
            if (i == items.size())
            {
                if (i == r.items.size())
                    return 0;
                return -1;
            }
            if (i == r.items.size())
                return 1;
            if (holds_alternative<int>(items[i]))
            {
                if (holds_alternative<int>(r.items[i]))
                {
                    int rv = get<int>(r.items[i]), lv = get<int>(items[i]);
                    if (lv < rv)
                        return -1;
                    if (lv > rv)
                        return 1;
                    continue;
                }
                else
                {
                    items[i] = fromInt(get<int>(items[i]));
                }
            }
            else
            {
                if (holds_alternative<int>(r.items[i]))
                {
                    r.items[i] = fromInt(get<int>(r.items[i]));
                }
            }
            int c = get<cPacket>(items[i]).compare(get<cPacket>(r.items[i]));
            if (c != 0)
                return c;
        }
    }

    static cPacket parse(cStringVector::iterator& i)
    {
        cPacket packet;
        for (;*i != "]";++i)
        {
            if (*i == "[")
            {
                ++i;
                packet.items.emplace_back(parse(i));
            }
            else
                packet.items.emplace_back(stoi(*i));
        }
        return packet;
    }
    static cPacket create(const string& s)
    {
        auto s2=replaceAll(replaceAll(replaceAll(replaceAll(replaceAll(s, "[[", "[,["), "]]", "],]"), "[]", "[,]"), "[", ",[,"), "]", ",],");
        auto tokens = cStringVector(s2, ",", false);
        auto b = tokens.begin();
        ++b;
        return parse(b);
    }
    static cPacket fromInt(int x)
    {
        cPacket p;
        p.items.emplace_back(x);
        return p;
    }
};

void prepare()
{
}

void Solve()
{
    cFastFileReader in("in.txt");

    int idx = 1;
    int result = 0;

    for (auto line : in)
    {
        auto left = cPacket::create(line);
        line = in.GetNextLine();
        auto right = cPacket::create(line);
        if (left.compare(right)<0)
        {
            result += idx;
        }
        ++idx;
    }
    printf("result part 1: %d\n", result);
}

void Solve2()
{
    cFastFileReader in("in.txt");

    vector<cPacket> packets;

    for (auto line : in)
    {
        packets.emplace_back(cPacket::create(line));
    }
    packets.emplace_back(cPacket::create("[[2]]"));
    packets.back().isDivider = true;
    packets.emplace_back(cPacket::create("[[6]]"));
    packets.back().isDivider = true;
    sort(ALL(packets), [](cPacket& l, cPacket& r) { return l.compare(r) < 0; });
    int result = 1;
    for (int i = 0; i < packets.size(); ++i)
    {
        if (packets[i].isDivider)
        {
            printf("----\n");
            result *= i + 1;
        }
        else
            printf(".\n");
    }
    printf("result part 2: %d\n", result);
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
