
#define _CRT_SECURE_NO_WARNINGS
//#include <windows.h>
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

using cPoint = array<int, 3>;
using cCube = array<cPoint, 2>;
using cInstruction = pair<pair<bool, int>, cCube>;
template<int d> void solve(vector<cInstruction> instructions);

ll allSize = 0, innerSize = 0;

template<> void solve<3>(vector<cInstruction> instructions)
{
    auto& last = instructions.back();
    if (last.first.first)
    {
        ll size = 1;
        bool inner = true;
        FOR(d, 3)
        {
            size *= last.second[1][d] - last.second[0][d];
            if (last.second[0][d] < -50 || last.second[0][d] > 50)
                inner = false;
        }
        allSize += size;
        if (inner)
            innerSize += size;
    }
}

template<int d> void solve(vector<cInstruction> instructions)
{
    vector<int> cuts;
    cuts.reserve(instructions.size() * 2);
    for (auto& i : instructions)
    {
        cuts.emplace_back(i.second[0][d]);
        cuts.emplace_back(i.second[1][d]);
    }
    sort(ALL(cuts));
    cuts.erase(unique(ALL(cuts)), cuts.end());
    unordered_map<int, vector<cInstruction>> nis;
    nis.reserve(instructions.size() * 3); // just a guess, but was a measurable speedup
    for (auto& i : instructions)
    {
        cCube cube = i.second;
        for (auto j = lower_bound(ALL(cuts), cube[0][d]);; ++j)
        {
            auto c = *j;
            if (cube[0][d] >= c)
                continue;
            if (cube[1][d] == c)
            {
                nis[cube[0][d]].emplace_back(i.first, cube);
                break;
            }
            cCube left = cube;
            left[1][d] = c;
            cube[0][d] = c;
            nis[left[0][d]].emplace_back(i.first, left);
        }
    }
    for (auto const& [c, sub_instructions] : nis)
        solve<d + 1>(std::move(sub_instructions));
}

ll Solve()
{
    RI(n);
    vector<cInstruction> instructions;
    FOR(i, n)
    {
        RI(onoffn);
        RII(x0, x1);
        RII(y0, y1);
        RII(z0, z1);
        pair<bool, int> onoff(onoffn == 1, (int)instructions.size());
        instructions.emplace_back(onoff, cCube { cPoint{ x0, y0, z0} , cPoint{ x1 + 1, y1 + 1, z1 + 1} });
    }
    solve<0>(std::move(instructions));
    return 0;
}



int Init()
{
    constexpr int t = 1;
    //RI(t);
    return t;
}

int main()
{
    int t = Init();
    for (int tc = 1; tc <= t; ++tc)
    {
        //       printf("Case #%d: ", tc);
        ll result = Solve();
        printf("result: %lld\n", result);

        printf("\n\n\ninner size: %lld\nall size: %lld\n\n", innerSize, allSize);
        //         RI(verdict);
        //         if (verdict == -1)
        //             break;
    }
}




