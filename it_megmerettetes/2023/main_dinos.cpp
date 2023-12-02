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
#include <filesystem>
namespace fs = std::filesystem;

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
#define RI(var_name) int var_name; fscanf(f, "%d", &var_name);
#define RIV(var_name, size) vector<int> var_name(size); for(auto &item_of_##var_name: var_name) fscanf(f, "%d", &item_of_##var_name);
#define RII(var_name1, var_name2) int var_name1, var_name2; fscanf(f, "%d %d", &var_name1, &var_name2);
#define RIII(var_name1, var_name2, var_name3) int var_name1, var_name2, var_name3; fscanf(f, "%d %d %d", &var_name1, &var_name2, &var_name3);
#define RIIII(var_name1, var_name2, var_name3, var_name4) int var_name1, var_name2, var_name3, var_name4; fscanf(f, "%d %d %d %d", &var_name1, &var_name2, &var_name3, &var_name4);
#define RL(var_name) ll var_name; fscanf(f, "%lld", &var_name);
#define RLV(var_name, size) vector<ll> var_name(size); for(auto &item_of_##var_name: var_name) fscanf(f, "%lld", &item_of_##var_name);
#define RLUV(var_name, size) vector<ull> var_name(size); for(auto &item_of_##var_name: var_name) fscanf(f, "%llu", &item_of_##var_name);
#define RLL(var_name1, var_name2) ll var_name1, var_name2; fscanf(f, "%lld %lld", &var_name1, &var_name2);
#define RLLL(var_name1, var_name2, var_name3) ll var_name1, var_name2, var_name3; fscanf(f, "%lld %lld %lld", &var_name1, &var_name2, &var_name3);
#define RLLLL(var_name1, var_name2, var_name3, var_name4) ll var_name1, var_name2, var_name3, var_name4; fscanf(f, "%lld %lld %lld %lld", &var_name1, &var_name2, &var_name3, &var_name4);
#define RD(var_name) double var_name; fscanf(f, "%lf", &var_name);
#define RDV(var_name, size) vector<double> var_name(size); for(auto &item_of_##var_name: var_name) fscanf(f, "%lf", &item_of_##var_name);
#define RDD(var_name1, var_name2) double var_name1, var_name2; fscanf(f, "%lf %lf", &var_name1, &var_name2);
#define RDDD(var_name1, var_name2, var_name3) double var_name1, var_name2, var_name3; fscanf(f, "%lf %lf %lf", &var_name1, &var_name2, &var_name3);
#define ALL(cont) cont.begin(), cont.end()
#define FOR(var, max_value) for(decltype(max_value) var=0;var<max_value;++var)

constexpr ll mod = 1'000'000'007;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



const int tab32[32] = {
    0,  9,  1, 10, 13, 21,  2, 29,
    11, 14, 16, 18, 22, 25,  3, 30,
    8, 12, 20, 28, 15, 17, 24,  7,
    19, 27, 23,  6, 26,  5,  4, 31 };

int log2_32(uint32_t value)
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return tab32[(uint32_t)(value * 0x07C4ACDD) >> 27];
}

const int MAX_LOG_N = 23; ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CHECK THIS!!!!!!

class cRangeMinimumQuery
{
    vector<vector<int>> mins;
public:
    void Init(const vector<int>& a)
    {
        int n = (int)a.size();
        mins.resize(MAX_LOG_N);
        for (auto& v : mins)
            v.resize(n);
        for (int i = 0; i < n; ++i)
        {
            mins[0][i] = a[i];
        }
        for (int j = 1, rs2 = 1; j < MAX_LOG_N; ++j, rs2 <<= 1)
        {
            for (int i = 0; i < n; ++i)
            {
                if (i + rs2 < n)
                    mins[j][i] = max(mins[j - 1][i], mins[j - 1][i + rs2]);   // MIN / MAX
                else
                    mins[j][i] = mins[j - 1][i];
            }
        }
    }
    int GetRMQ(int left, int right)
    {
        int range_size = right - left + 1;
        int l32 = log2_32(range_size);
        int queried_range_size = 1 << l32, query_index = l32;
        return max(mins[query_index][left], mins[query_index][right - queried_range_size + 1]);
    }
};




void Solve(FILE *f)
{
    vector<pair<int, int>> ds;

    for (;;)
    {
        int a, b;
        fscanf(f, "%d %d", &a, &b);
        if (a == -1 || b == -1)
            break;
        ds.emplace_back(a, b);
    }
    sort(ALL(ds));
    int n = (int)ds.size();

    vector<int> rmq_v(ds.size());
    for (size_t i = 0; i < ds.size(); ++i)
    {
        rmq_v[i] = ds[i].second;
    }

    cRangeMinimumQuery rmq;
    rmq.Init(rmq_v);

    int eatable = 0;
    for (int i = 0; i < n; ++i)
    {
        int firstDiffA = i + 1;
        while (firstDiffA < n && ds[i].first == ds[firstDiffA].first)
            ++firstDiffA;
        if (firstDiffA == n)
            break;

        if (rmq.GetRMQ(firstDiffA, n - 1) > ds[i].second)
            ++eatable;
    }

    printf("N: %d\n", n);
    printf("non-eatable: %d\n", n - eatable);
}

int main()
{
    for (const auto& entry : fs::directory_iterator("dinos"))
    {
        if (fs::is_regular_file(entry) && entry.path().extension() == ".txt")
        {
            printf("\n--------------------------------------------------------\n-=#=- %s -=#=-\n", entry.path().string().c_str());
            FILE* f = fopen(entry.path().string().c_str(), "r");
            Solve(f);
            fclose(f);
        }
    }

    return 0;
}




