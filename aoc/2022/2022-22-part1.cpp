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

int steps[] =
{
12, 'R',25, 'R',28, 'R',21, 'L',15, 'R',49, 'L',21, 'R',10, 'R',27, 'R',28, 'L',47, 'L',18, 'R',43, 'L',1, 'R',8, 'L',34, 'R',4, 'R',34, 'L',48, 'L',42, 'L',19, 'R',32, 'R',30, 'R',34, 'L',11, 'L',49, 'L',6, 'R',49, 'L',5, 'R',45, 'R',11, 'L',6, 'R',47, 'L',27, 'L',12, 'R',46, 'L',40, 'R',46, 'L',33, 'L',14, 'R',18, 'R',41, 'R',12, 'L',25, 'L',35, 'L',29, 'L',16, 'R',41, 'L',14, 'R',32, 'L',29, 'L',49, 'L',24, 'L',35, 'R',39, 'L',8, 'R',34, 'L',20, 'L',19, 'L',14, 'R',44, 'R',12, 'L',35, 'R',10, 'L',35, 'L',11, 'L',46, 'R',35, 'R',14, 'R',21, 'L',10, 'R',10, 'L',31, 'L',2, 'L',47, 'L',10, 'L',40, 'R',43, 'L',8, 'L',8, 'L',38, 'L',6, 'L',49, 'L',27, 'L',14, 'L',21, 'L',49, 'L',45, 'L',29, 'L',7, 'L',28, 'L',41, 'L',10, 'L',4, 'R',28, 'R',7, 'R',48, 'L',5, 'R',6, 'L',25, 'L',47, 'R',40, 'R',30, 'R',34, 'L',39, 'R',35, 'L',46, 'L',39, 'R',24, 'L',45, 'L',26, 'R',44, 'R',13, 'L',36, 'L',15, 'R',35, 'R',7, 'R',42, 'R',34, 'L',3, 'L',28, 'R',42, 'R',2, 'R',32, 'R',42, 'R',22, 'R',23, 'R',9, 'R',34, 'R',32, 'R',34, 'L',10, 'L',22, 'R',10, 'L',31, 'L',17, 'L',23, 'L',45, 'R',14, 'R',2, 'R',34, 'R',10, 'R',45, 'L',28, 'R',30, 'L',25, 'R',20, 'R',41, 'L',1, 'L',29, 'R',31, 'R',11, 'R',37, 'R',18, 'L',50, 'L',50, 'R',12, 'L',33, 'L',6, 'R',49, 'L',50, 'R',12, 'R',2, 'L',3, 'R',42, 'R',9, 'L',7, 'L',44, 'R',30, 'L',26, 'L',1, 'R',37, 'R',22, 'R',18, 'R',50, 'R',17, 'L',23, 'L',5, 'R',8, 'R',48, 'L',30, 'R',33, 'L',2, 'R',46, 'L',49, 'L',1, 'L',18, 'R',37, 'R',50, 'L',10, 'R',8, 'L',7, 'R',8, 'R',37, 'R',30, 'R',14, 'R',40, 'L',36, 'L',15, 'L',41, 'L',46, 'R',39, 'R',8, 'L',4, 'L',34, 'R',11, 'L',25, 'R',37, 'L',23, 'R',18, 'R',47, 'L',23, 'R',32, 'L',26, 'R',49, 'R',16, 'R',30, 'R',12, 'R',39, 'L',7, 'L',43, 'R',38, 'R',45, 'L',50, 'L',35, 'R',46, 'L',24, 'L',41, 'R',3, 'R',18, 'R',5, 'L',12, 'L',39, 'R',41, 'L',46, 'R',31, 'L',12, 'L',48, 'L',7, 'R',21, 'L',22, 'R',5, 'L',27, 'R',42, 'L',26, 'R',20, 'R',40, 'R',9, 'R',8, 'R',22, 'R',6, 'R',41, 'R',40, 'L',33, 'L',24, 'L',30, 'L',45, 'L',29, 'R',18, 'R',20, 'R',5, 'L',31, 'L',4, 'L',40, 'L',49, 'R',37, 'R',9, 'L',17, 'R',4, 'R',34, 'R',47, 'L',18, 'L',48, 'L',4, 'L',8, 'R',4, 'L',5, 'L',40, 'L',7, 'R',47, 'L',29, 'R',49, 'R',50, 'R',50, 'L',22, 'R',23, 'R',21, 'L',50, 'L',17, 'L',28, 'L',41, 'L',38, 'R',6, 'R',36, 'L',12, 'L',9, 'R',9, 'R',40, 'L',16, 'L',4, 'R',44, 'L',7, 'L',37, 'R',14, 'L',10, 'R',16, 'R',38, 'R',44, 'L',35, 'R',19, 'R',47, 'R',18, 'R',29, 'L',31, 'L',40, 'L',47, 'R',35, 'L',42, 'L',7, 'L',34, 'L',49, 'R',10, 'L',29, 'L',14, 'R',12, 'R',24, 'R',13, 'L',29, 'L',12, 'R',45, 'R',8, 'R',50, 'R',39, 'R',34, 'L',50, 'L',10, 'R',44, 'L',32, 'L',31, 'L',24, 'L',30, 'R',47, 'R',41, 'R',2, 'L',7, 'R',3, 'R',31, 'R',38, 'R',33, 'R',35, 'R',46, 'L',22, 'R',12, 'R',47, 'L',20, 'L',36, 'R',49, 'R',27, 'L',3, 'R',25, 'L',2, 'L',12, 'R',13, 'L',28, 'L',17, 'L',29, 'L',44, 'L',18, 'L',35, 'L',18, 'R',6, 'R',22, 'R',24, 'L',38, 'L',31, 'R',23, 'R',26, 'L',39, 'R',40, 'R',13, 'L',37, 'L',24, 'R',1, 'R',14, 'L',20, 'L',14, 'R',43, 'R',14, 'R',45, 'R',36, 'R',22, 'R',47, 'R',22, 'L',18, 'R',17, 'L',28, 'L',45, 'R',22, 'L',20, 'L',19, 'R',37, 'R',20, 'L',43, 'L',43, 'L',14, 'L',8, 'R',16, 'R',6, 'L',13, 'R',6, 'L',23, 'R',16, 'L',1, 'L',45, 'R',9, 'R',36, 'R',17, 'L',45, 'L',30, 'R',43, 'L',31, 'L',38, 'L',4, 'L',28, 'L',27, 'R',12, 'L',7, 'R',27, 'R',50, 'R',49, 'L',4, 'L',30, 'L',21, 'R',27, 'R',46, 'L',44, 'R',17, 'L',15, 'L',5, 'L',4, 'L',29, 'R',31, 'R',19, 'L',13, 'R',48, 'L',36, 'L',8, 'L',17, 'R',23, 'R',1, 'R',40, 'L',33, 'R',50, 'R',12, 'L',36, 'L',19, 'L',6, 'L',45, 'L',17, 'L',14, 'R',12, 'R',3, 'L',36, 'L',35, 'L',19, 'R',46, 'R',21, 'L',8, 'L',33, 'L',11, 'L',34, 'L',25, 'L',2, 'R',32, 'R',10, 'R',30, 'R',15, 'R',46, 'R',37, 'R',24, 'R',39, 'R',7, 'R',39, 'R',43, 'L',29, 'R',39, 'L',34, 'R',29, 'R',19, 'R',49, 'R',41, 'L',38, 'R',11, 'L',43, 'R',17, 'L',35, 'L',45, 'R',41, 'L',22, 'R',48, 'R',25, 'R',8, 'R',45, 'R',39, 'R',13, 'R',8, 'L',36, 'R',2, 'R',5, 'R',50, 'L',40, 'L',12, 'L',23, 'R',42, 'L',2, 'R',41, 'R',14, 'R',19, 'R',28, 'L',25, 'L',49, 'R',39, 'R',9, 'R',13, 'L',43, 'L',20, 'L',28, 'L',46, 'R',19, 'R',36, 'R',46, 'R',12, 'L',9, 'R',7, 'R',30, 'R',30, 'R',9, 'R',43, 'R',26, 'R',36, 'L',11, 'L',34, 'R',41, 'L',42, 'L',19, 'R',2, 'L',18, 'R',9, 'R',20, 'R',11, 'L',11, 'L',2, 'L',10, 'L',24, 'R',25, 'R',32, 'L',44, 'R',14, 'R',4, 'R',24, 'R',10, 'R',24, 'L',5, 'R',4, 'L',13, 'R',35, 'R',35, 'R',22, 'L',18, 'L',24, 'R',46, 'L',32, 'L',47, 'L',37, 'R',21, 'R',49, 'R',6, 'R',12, 'L',16, 'L',31, 'L',39, 'L',26, 'L',48, 'L',8, 'L',26, 'R',27, 'R',16, 'L',48, 'L',30, 'R',36, 'L',24, 'L',48, 'R',36, 'L',1, 'L',1, 'R',16, 'L',38, 'R',1, 'R',26, 'L',11, 'L',29, 'R',20, 'L',50, 'R',22, 'R',44, 'R',4, 'L',18, 'R',37, 'L',6, 'L',19, 'L',44, 'R',20, 'R',14, 'R',37, 'R',3, 'L',39, 'R',12, 'L',23, 'L',46, 'L',19, 'L',45, 'R',35, 'L',12, 'L',7, 'L',28, 'R',46, 'L',18, 'R',25, 'R',38, 'R',10, 'L',22, 'R',18, 'R',34, 'L',25, 'R',4, 'L',15, 'L',2, 'L',9, 'L',35, 'L',24, 'R',6, 'R',27, 'R',22, 'R',32, 'L',22, 'L',49, 'R',22, 'L',39, 'L',27, 'L',38, 'R',38, 'R',12, 'R',28, 'L',27, 'R',2, 'R',14, 'R',10, 'R',30, 'L',25, 'R',4, 'L',15, 'L',40, 'R',34, 'R',42, 'R',38, 'R',9, 'R',12, 'L',14, 'L',15, 'R',34, 'R',6, 'R',9, 'R',41, 'R',11, 'R',47, 'R',23, 'R',25, 'R',37, 'R',31, 'L',21, 'R',37, 'R',4, 'R',15, 'L',20, 'R',37, 'R',33, 'L',13, 'R',23, 'R',38, 'R',23, 'R',42, 'R',37, 'L',17, 'L',34, 'R',21, 'R',36, 'R',21, 'R',39, 'R',14, 'L',35, 'R',3, 'R',9, 'L',33, 'R',45, 'R',37, 'R',32, 'R',18, 'R',26, 'L',27, 'R',39, 'R',50, 'R',37, 'R',34, 'L',41, 'R',48, 'L',43, 'L',37, 'L',23, 'L',42, 'L',8, 'L',2, 'R',25, 'R',16, 'L',6, 'R',11, 'L',2, 'L',5, 'L',41, 'L',25, 'L',6, 'L',41, 'L',46, 'L',1, 'R',25, 'L',31, 'R',26, 'L',25, 'L',26, 'R',18, 'L',46, 'R',1, 'R',14, 'R',15, 'R',29, 'R',29, 'R',34, 'R',19, 'R',28, 'L',50, 'R',35, 'R',10, 'R',10, 'L',41, 'L',4, 'R',15, 'R',36, 'L',7, 'L',36, 'L',7, 'R',36, 'L',40, 'R',16, 'L',33, 'L',16, 'L',23, 'R',45, 'R',32, 'R',43, 'L',10, 'L',42, 'R',11, 'L',7, 'R',18, 'L',17, 'R',14, 'R',25, 'L',47, 'L',38, 'R',46, 'L',28, 'R',21, 'L',43, 'R',29, 'R',44, 'L',20, 'R',45, 'L',29, 'R',3, 'L',1, 'L',38, 'R',32, 'R',42, 'L',15, 'L',46, 'L',22, 'R',22, 'L',26, 'R',42, 'R',12, 'R',38, 'R',3, 'L',5, 'R',4, 'R',17, 'L',1, 'L',34, 'R',33, 'R',9, 'L',39, 'R',32, 'R',22, 'R',19, 'R',43, 'L',50, 'L',32, 'R',15, 'R',21, 'L',14, 'L',35, 'L',7, 'L',31, 'R',9, 'R',28, 'R',50, 'R',25, 'R',13, 'L',22, 'L',32, 'R',46, 'L',37, 'L',16, 'L',39, 'L',19, 'L',31, 'R',17, 'R',47, 'R',29, 'R',21, 'L',24, 'L',49, 'L',9, 'R',48, 'L',13, 'R',26, 'L',43, 'L',35, 'L',5, 'L',4, 'R',45, 'L',38, 'R',29, 'R',1, 'L',40, 'L',5, 'R',50, 'R',1, 'L',49, 'R',46, 'L',40, 'L',30, 'R',41, 'R',4, 'L',11, 'R',40, 'R',35, 'L',14, 'L',15, 'R',21, 'R',17, 'L',22, 'L',18, 'R',37, 'R',49, 'R',3, 'L',21, 'R',22, 'L',36, 'L',14, 'R',43, 'R',9, 'L',24, 'L',39, 'R',11, 'L',39, 'L',6, 'L',49, 'R',46, 'R',39, 'L',15, 'L',37, 'L',44, 'R',25, 'R',48, 'R',46, 'L',38, 'R',7, 'L',12, 'R',18, 'R',12, 'R',14, 'R',44, 'R',37, 'R',23, 'R',42, 'R',37, 'L',50, 'L',9, 'L',27, 'R',36, 'L',18, 'R',14, 'R',3, 'L',5, 'R',38, 'R',44, 'L',35, 'R',36, 'L',12, 'L',41, 'L',15, 'R',6, 'L',39, 'L',40, 'R',5, 'L',2, 'R',18, 'R',36, 'R',38, 'L',26, 'R',38, 'L',22, 'R',21, 'R',5, 'R',1, 'L',11, 'L',29, 'R',2, 'L',34, 'L',41, 'L',29, 'R',32, 'R',25, 'L',37, 'R',3, 'L',34, 'L',18, 'L',14, 'L',33, 'L',17, 'R',48, 'L',47, 'L',12, 'R',26, 'R',35, 'L',22, 'L',16, 'R',20, 'R',24, 'R',22, 'R',38, 'R',18, 'R',13, 'R',37, 'R',31, 'R',22, 'R',42, 'R',37, 'R',34, 'R',45, 'R',21, 'L',27, 'L',14, 'L',29, 'L',16, 'R',15, 'L',1, 'L',9, 'R',49, 'L',14, 'R',40, 'R',33, 'R',3, 'L',30, 'L',32, 'L',32, 'R',16, 'L',22, 'R',44, 'L',43, 'R',22, 'R',39, 'L',9, 'L',50, 'L',16, 'R',9, 'R',18, 'L',12, 'R',27, 'R',10, 'R',10, 'R',27, 'L',27, 'R',40, 'R',10, 'L',46, 'R',32, 'L',44, 'L',9, 'L',48, 'R',28, 'R',29, 'R',23, 'L',45, 'L',26, 'L',37, 'L',38, 'L',29, 'R',27, 'R',35, 'R',19, 'L',46, 'R',44, 'R',13, 'R',2, 'L',27, 'R',46, 'R',48, 'L',42, 'R',27, 'R',43, 'R',27, 'R',23, 'R',20, 'R',23, 'L',40, 'L',36, 'R',29, 'R',2, 'L',22, 'R',15, 'R',14, 'R',33, 'L',45, 'R',14, 'R',32, 'R',1, 'R',43, 'L',20, 'R',20, 'L',15, 'L',44, 'L',22, 'L',4, 'L',24, 'R',22, 'L',23, 'R',29, 'R',34, 'L',28, 'L',15, 'R',48, 'L',28, 'L',26, 'L',26, 'R',16, 'L',40, 'L',11, 'L',27, 'R',29, 'L',27, 'R',28, 'L',5, 'L',6, 'R',30, 'L',8, 'R',35, 'R',44, 'L',29, 'L',48, 'L',11, 'L',29, 'R',2, 'L',21, 'R',10, 'R',29, 'L',45, 'L',31, 'L',16, 'L',12, 'R',16, 'R',14, 'R',25, 'R',49, 'R',17, 'L',41, 'R',31, 'R',2, 'R',45, 'L',11, 'R',10, 'R',41, 'R',34, 'R',12, 'R',39, 'R',9, 'R',6, 'R',9, 'L',29, 'R',19, 'R',43, 'R',3, 'R',28, 'L',29, 'L',9, 'R',27, 'R',3, 'R',9, 'L',40, 'R',36, 'R',47, 'L',9, 'L',11, 'L',44, 'L',31, 'R',16, 'L',50, 'R',32, 'L',7, 'L',49, 'R',26, 'R',33, 'R',48, 'L',49, 'L',34, 'R',43, 'R',1, 'L',14, 'L',1, 'R',19, 'R',4, 'L',15, 'R',11, 'R',34, 'L',13, 'R',36, 'L',2, 'R',32, 'L',33, 'R',19, 'L',16, 'L',22, 'R',6, 'R',9, 'L',23, 'R',25, 'L',9, 'L',35, 'L',46, 'R',15, 'L',26, 'L',7, 'R',40, 'L',35, 'R',45, 'L',49, 'L',47, 'R',27, 'R',3, 'R',39, 'R',17, 'R',46, 'L',47, 'R',46, 'L',23, 'L',36, 'R',50, 'L',16, 'R',45, 'R',1, 'L',4, 'R',39, 'R',32, 'L',33, 'R',18, 'L',35, 'R',41, 'R',44, 'L',20, 'R',39, 'L',37, 'L',14, 'L',33, 'L',23, 'L',23, 'R',22, 'L',13, 'L',14, 'R',43, 'R',31, 'R',2, 'L',24, 'L',26, 'R',49, 'L',38, 'R',38, 'L',25, 'L',28, 'L',29, 'R',50, 'L',16, 'L',31, 'L',20, 'R',38, 'R',42, 'L',7, 'R',20, 'L',42, 'L',12, 'R',39, 'L',35, 'L',16, 'L',11, 'L',22, 'L',28, 'L',7, 'L',16, 'L',50, 'L',3, 'L',42, 'L',38, 'R',23, 'R',37, 'R',30, 'R',48, 'L',16, 'R',36, 'R',49, 'R',27, 'R',19, 'R',39, 'R',14, 'L',35, 'L',12, 'R',14, 'R',14, 'R',1, 'R',19, 'L',50, 'R',28, 'R',26, 'L',33, 'L',28, 'R',35, 'L',25, 'R',24, 'R',48, 'R',27, 'L',44, 'L',34, 'L',46, 'R',49, 'L',6, 'R',24, 'R',19, 'L',7, 'L',39, 'R',6, 'L',28, 'L',43, 'L',14, 'L',40, 'R',10, 'L',27, 'R',12, 'R',45, 'L',4, 'L',7, 'R',35, 'L',38, 'R',45, 'L',50, 'L',8, 'L',33, 'L',22, 'R',28, 'R',27, 'R',30, 'R',30, 'R',3, 'L',23, 'R',28, 'R',9, 'L',39, 'L',7, 'L',21, 'L',40, 'L',18, 'R',43, 'R',50, 'R',31, 'R',33, 'L',26, 'L',34, 'R',25, 'L',1, 'L',43, 'L',21, 'L',50, 'R',14, 'L',20, 'R',2, 'R',4, 'L',13, 'L',24, 'R',38, 'L',10, 'L',43, 'R',34, 'L',30, 'R',21, 'L',20, 'R',12, 'R',12, 'R',16, 'R',23, 'L',21, 'L',46, 'R',39, 'R',32, 'R',7, 'R',20, 'R',19, 'R',37, 'L',48, 'L',12, 'L',40, 'R',14, 'L',33, 'R',17, 'L',32, 'R',24, 'L',28, 'L',45, 'R',17, 'L',13, 'R',6, 'L',9, 'R',28, 'L',46, 'L',35, 'R',49, 'R',9, 'L',4, 'L',43, 'L',38, 'L',22, 'R',15, 'L',48, 'L',11, 'R',38, 'L',24, 'R',5, 'R',2, 'R',49, 'R',43, 'R',12, 'L',18, 'R',21, 'R',36, 'L',24, 'R',3, 'L',30, 'R',10, 'L',30, 'R',47, 'R',48, 'R',46, 'R',31, 'L',9, 'L',45, 'R',1, 'L',48, 'R',21, 'R',14, 'R',5, 'R',2, 'L',46, 'L',10, 'L',27, 'L',22, 'R',15, 'R',34, 'R',42, 'L',26, 'R',43, 'R',17, 'L',36, 'R',13, 'R',14, 'R',44, 'L',24, 'L',34, 'L',9, 'L',29, 'L',38, 'R',16, 'L',40, 'R',39, 'R',29, 'L',15, 'R',42, 'L',18, 'L',3, 'R',40, 'L',30, 'R',40, 'L',2, 'L',29, 'R',38, 'L',43, 'R',32, 'R',34, 'L',3, 'L',4, 'R',32, 'L',28, 'L',49, 'L',4, 'R',42, 'R',37, 'L',16, 'L',46, 'R',27, 'L',15, 'R',5, 'R',28, 'L',36, 'R',24, 'L',27, 'R',22, 'L',37, 'L',23, 'R',21, 'R',1, 'R',24, 'L',13, 'R',3, 'L',43, 'R',8, 'L',30, 'R',16, 'R',36, 'L',46, 'L',21, 'L',46, 'L',27, 'L',30, 'L',2, 'R',4, 'L',8, 'R',34, 'R',1, 'R',2, 'L',9, 'R',8, 'R',18, 'L',50, 'L',13, 'L',26, 'L',37, 'L',18, 'L',43, 'L',42, 'R',39, 'R',33, 'R',27, 'R',24, 'L',49, 'R',24, 'L',9, 'L',50, 'L',45, 'L',32, 'R',33, 'R',10, 'L',12, 'R',28, 'R',15, 'R',14, 'L',44, 'L',45, 'L',20, 'R',41, 'L',5, 'R',30, 'L',33, 'R',15, 'L',22, 'L',41, 'R',46, 'R',20, 'L',36, 'R',23, 'R',2, 'L',29, 'L',45, 'L',26, 'R',44, 'R',25, 'R',21, 'R',6, 'R',45, 'R',3, 'L',41, 'L',49, 'L',48, 'L',43, 'R',43, 'R',49, 'R',6, 'R',17, 'L',48, 'R',7, 'R',21, 'L',36, 'L',49, 'L',15, 'R',43, 'R',36, 'L',28, 'R',40, 'L',9, 'R',23, 'L',19, 'L',10, 'R',4, 'R',14, 'L',20, 'L',7, 'L',21, 'L',46, 'R',12, 'R',28, 'R',32, 'L',39, 'R',4, 'R',19, 'L',25, 'R',7, 'L',34, 'R',38, 'R',22, 'L',7, 'L',5, 'L',22, 'R',44, 'R',20, 'R',10, 'R',24, 'L',45, 'R',45, 'L',11, 'L',48, 'L',23, 'L',8, 'L',34, 'R',27, 'L',17, 'R',27, 'L',47, 'L',2, 'L',9, 'L',49, 'R',1, 'R',32, 'R',40, 'L',39, 'R',1, 'R',18, 'R',50, 'L',29, 'L',19, 'R',32, 'R',38, 'L',26, 'L',17, 'R',36, 'R',40, 'R',31, 'L',29, 'L',29, 'R',44, 'L',25, 'R',37, 'R',3, 'R',13, 'L',43, 'L',18, 'R',33, 'R',5, 'R',9, 'R',31, 'L',23, 'L',17, 'R',1, 'R',10, 'R',37, 'R',12, 'L',21, 'L',6, 'R',47, 'R',38, 'R',44, 'R',3, 'L',11, 'R',13, 'L',41, 'L',24, 'R',39, 'L',14, 'L',7, 'R',21, 'L',25, 'R',39, 'R',45, 'R',44, 'L',34, 'L',1, 'L',41, 'R',7, 'L',44, 'L',8, 'L',19, 'R',30, 'R',7, 'L',7, 'R',44, 'L',9, 'L',8, 'L',46, 'L',19, 'R',31, 'R',44, 'L',19, 'R',41, 'L',21, 'R',4, 'L',34, 'L',12, 'L',44, 'R',4, 'R',20, 'R',19, 'R',34, 'L',7, 'L',8, 'L',9, 'R',34, 'R',34, 'R',4, 'L',12, 'R',2, 'R',41, 'L',50, 'R',11, 'L',6, 'R',14, 'R',40, 'R',35, 'L',43, 'L',47, 'L',7, 'R',40, 'R',29, 'L',11, 'L',45, 'R',32, 'R',10, 'R',1, 'R',25, 'L',14, 'R',19, 'L',38, 'L',20, 'L',7, 'L',29, 'R',19, 'L',30, 'R',3, 'R',23, 'L',34, 'R',40, 'L',34, 'R',2, 'R',40, 'L',46, 'R',8, 'R',27, 'R',32, 'R',45, 'R',39, 'L',13, 'L',37, 'L',40, 'R',22, 'L',50, 'R',49, 'R',10, 'L',22, 'R',3, 'L',27, 'L',11, 'L',34, 'R',35, 'L',4, 'R',1, 'R',38, 'R',20, 'L',2, 'L',48, 'R',21, 'L',15, 'R',5, 'R',11, 'L',37, 'L',44, 'L',26, 'L',30, 'L',7, 'R',39, 'R',50, 'L',43, 'L',41, 'L',22, 'R',7, 'L',47, 'R',7, 'L',18, 'R',9, 'R',30, 'R',50, 'L',22, 'R',37, 'R',8, 'R',17, 'R',45, 'R',41, 'R',16, 'R',23, 'L',28, 'L',13, 'L',7, 'R',41, 'L',24, 'L',7, 'L',30, 'R',47, 'L',15, 'L',49, 'L',44, 'R',33, 'R',48, 'L',47, 'R',38, 'L',21, 'L',37, 'R',2, 'R',29, 'L',32, 'L',13, 'L',6, 'R',30, 'R',42, 'R',42, 'L',12, 'R',8, 'R',36, 'R',6, 'R',13, 'R',22, 'R',42, 'L',44, 'R',6, 'R',4, 'R',19, 'L',22, 'R',15, 'L',27, 'L',41, 'L',13, 'L',26, 'R',35, 'L',11, 'L',15, 'L',31, 'R',17, 'L',34, 'R',6, 'R',20, 'R',41, 'R',24, 'L',22, 'R',40, 'R',7, 'L',20, 'L',20, 'R',28, 'L',43, 'L',15, 'R',35, 'R',5, 'L',41, 'R',40, 'L',8, 'L',25, 'L',43, 'R',45, 'R',39, 'L',41, 'R',5, 'L',17, 'R',32, 'R',19, 'L',21, 'R',1, 'R',15, 'L',8, 'L',20, 'R',13, 'R',46, 'L',12, 'R',6, 'L',16, 'R',49, 'L',39, 'R',43, 'L',31, 'R',24, 'R',29, 'R',48, 'L',23, 'L',19, 'L',44, 'R',32, 'L',25, 'L',2, 'L',6, 'R',11, 'L',11, 'L',31, 'R',39, 'R',20, 'R',30, 'L',7, 'R',16, 'L',17, 'R',42, 'L',4, 'L',44, 'L',8, 'L',26, 'L',39, 'L',41, 'L',45, 'L',16, 'R',37, 'R',6, 'L',35, 'R',33, 'L',42, 'L',16, 'R',50, 'R',21, 'L',43, 'R',22, 'R',25, 'L',43, 'R',46, 'L',45, 'L',12, 'R',40, 'L',13, 'L',3, 'L',9, 'R',32, 'R',15, 'R',21, 'R',20, 'L',12, 'L',4, 'L',7, 'L',13, 'L',48, 'L',39, 'L',9, 'R',19, 'R',48, 'R',3, 'L',31, 'R',39, 'R',49, 'L',35, 'R',2, 'R',5, 'L',33, 'R',26, 'R',43, 'L',11, 'R',49, 'R',20, 'L',30, 'R',12, 'L',31, 'R',43, 'R',36, 'R',49, 'L',30, 'L',47, 'R',18, 'L',9, 'L',48, 'R',47, 'L',11, 'R',29, 'L',1, 'R',45, 'R',37, 'L',29, 'L',50, 'R',39, 'L',16, 'R',27, 'L',26, 'L',19, 'L',21, 'R',9, 'R',20, 'R',28, 'R',20, 'R',8, 'R',2, 'R',39, 'R',31, 'L',2, 'R',14, 'L',36, 'L',39, 'L',10, 'R',8, 'R',40, 'L',40, 'R',14, 'R',32, 'L',33, 'L',23, 'L',38, 'L',46, 'R',34, 'L',19, 'R',35, 'R',36, 'L',20, 'R',16, 'L',49, 'L',17, 'R',48, 'R',12, 'R',15, 'R',21, 'L',6, 'L',45, 'L',26, 'L',19, 'R',43, 'R',5, 'R',20, 'R',19, 'L',21, 'L',44, 'R',31, 'R',6, 'R',16, 'R',49, 'R',34, 'L',34, 'R',49, 'R',41, 'R',25, 'R',47, 'R',11, 'L',29, 'R',34, 'R',20, 'L',29, 'L',35, 'L',20, 'L',11, 'R',15, 'L',50, 'L',17, 'R',2


//10,'R',5,'L',5,'R',10,'L',4,'R',5,'L',5
,'*'
};

char m[256 * 256];

using cPos = array<int, 2>;
cPos bottom_right;

cPos operator+(const cPos& l, const cPos& r)
{
    return { l[0] + r[0], l[1] + r[1] };
}
bool isValid(const cPos& p)
{
    return p[0] >= 0 && p[0] <= bottom_right[0] && p[1] >= 0 && p[1] <= bottom_right[1];
}
char& at(const cPos& p)
{
    return m[(p[0] + 1) * 256 + p[1] + 1];
}
char* row(int r)
{
    return m + (r + 1) * 256 + 1;
}


struct cDirection
{
    cPos offset;
    array<int,256> starts, ends;  // inclusive
    array<array<int,256>, 256> until_next_walls;
    int index_changed = 0, index_other = 0;
    void wrap(cPos& p)
    {
        int end = ends[p[index_other]];
        int start = starts[p[index_other]];
        if (p[index_changed] < start)
        {
            p[index_changed] = end + 1 + p[index_changed] - start;
        }
        else if (p[index_changed] > end)
        {
            p[index_changed] = p[index_changed] - end - 1 + start;
        }
    }


};

cDirection dirs[4];  //  > V < ^

void prepare()
{
    memset(m, ' ', sizeof(m));
    cFastFileReader in("in.txt");
    int num_rows=0, num_cols=0;
    for (auto line : in)
    {
        line.copyTo(row(num_rows++));
        num_cols = max(num_cols, line.Length);
    }
    bottom_right = { num_rows-1, num_cols-1 };

    dirs[0].offset = { 0, 1 }; dirs[0].index_changed = 1;
    dirs[1].offset = { 1, 0 }; dirs[1].index_other = 1;
    dirs[2].offset = { 0, -1 }; dirs[2].index_changed = 1;
    dirs[3].offset = { -1, 0 }; dirs[3].index_other = 1;

    auto findStartAndEnd = [&](cDirection& dir, cPos start, cPos next_offset, int n)
    {
        for (int i = 0; i < n; ++i)
        {
            bool found_start = false;
            cPos prev_p = start;
            for (cPos p = start; isValid(p); p = p + dir.offset)
            {
                if (at(p) != ' ')
                {
                    if (!found_start)
                    {
                        found_start = true;
                        dir.starts[i] = p[dir.index_changed];
                    }
                }
                else
                {
                    if (found_start)
                    {
                        break;
                    }
                }
                prev_p = p;
            }            
            dir.ends[i] = prev_p[dir.index_changed];
            start = start + next_offset;
        }
    };
    findStartAndEnd(dirs[0], { 0, 0 }, { 1, 0 }, num_rows);
    findStartAndEnd(dirs[1], { 0, 0 }, { 0, 1 }, num_cols);
    dirs[2].starts = dirs[0].starts;
    dirs[3].starts = dirs[1].starts;
    dirs[2].ends = dirs[0].ends;
    dirs[3].ends = dirs[1].ends;

    auto calculateRuns = [&](cDirection& dir, int n)
    {
        for (int i = 0; i < n; ++i)
        {
            cPos p = { 0,0 };
            p[dir.index_other] = i;
            p[dir.index_changed] = dir.starts[i];
            cPos offset = dir.offset;
            offset[0] *= -1; offset[1] *= -1;
            int until_wall = 0;
            for (int j = 0; j < n * 2; ++j)
            {
//                 printf("pos: %d, %d, until_wall: %d\n", p[0], p[1], until_wall);
//                 fflush(stdout);
                dir.until_next_walls[i][p[dir.index_changed]] = until_wall;
                ++until_wall;
                if (at(p) == '#')
                    until_wall = 0;
                else if (at(p) == ' ')
                {
                    int alma = 2;
                }
                p = p + offset;
                dir.wrap(p);
            }
        }
    };
    //  > V < ^
    calculateRuns(dirs[0], num_rows);
    calculateRuns(dirs[1], num_cols);
    calculateRuns(dirs[2], num_rows);
    calculateRuns(dirs[3], num_cols);
}

void Solve()    //  > V < ^
{
    int d = 0;
    cPos p = { 0, dirs[0].starts[0] };

//     for (int j = 0; j < 1'000'000; ++j)
    {

        for (int* s = steps;; s += 2)
        {
            cDirection& dir = dirs[d];
            int index_changed = dir.index_changed;
            int& changed = p[index_changed];
            int other = p[dir.index_other];
            int steps_allowed = min(s[0], dir.until_next_walls[other][changed]);
            printf("command: %d %c pos: %d, %d, dir: %c, steps_allowed: %d, start: %d, end: %d\n",
                s[0], s[1],
                p[0], p[1], ">V<^"[d],
                steps_allowed,
                
                dir.starts[other], dir.ends[other]);
            //         fflush(stdout);
            if (steps_allowed == 47)
            {
                int alma = 2;
            }
            changed += dir.offset[index_changed] * steps_allowed;
            dirs[d].wrap(p);
            if (s[1] == 'L')
            {
                if (-1 == --d)
                    d = 3;
            }
            else if (s[1] == 'R')
            {
                if (4 == ++d)
                    d = 0;
            }
            else
                break;

            if (at(p) == ' ')
            {
                printf("GOTCHA!\n");
            }
        }
    }
    // 96246 too high
    // 20250 too low
    printf("finished on: %d, %d, facing: %d\n", p[0] + 1, p[1] + 1, d);
    printf("result part 1: %d\n", (p[0] + 1) * 1000 + 4 * (p[1]+1) + d);
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