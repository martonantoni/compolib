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

char buf[100];

void Solve(FILE* f)
{
    std::vector<ll> left;
    std::deque<ll> right;
    int ls = 0, rs = 0;
    ll result = 0;
    for (;;)
    {
        fscanf(f, "%s", buf);
//         printf("Read: %s\n", buf);
        if (*buf == 'x')
            break;
        if (*buf == 'p')
        {
            if (ls == 0)
            {
                continue;
            }
            if (ls == rs)
            {
                left.push_back(right.front());
                right.pop_front();
                ++ls;
                --rs;

            }
//             printf("popping: %lld\n", left.back());
            result += left.back();
            left.pop_back();
            --ls;
            if (ls < rs)
            {
                left.push_back(right.front());
                right.pop_front();
                ++ls;
                --rs;
            }
        }
        else
        {
            ll x = atol(buf);
            right.push_back(x);
            ++rs;
            if (rs > ls)
            {
                left.push_back(right.front());
                right.pop_front();
                ++ls;
                --rs;
            }
        }
//         printf("state: ");
//         for (auto x : left) printf("%lld, ", x);
//         for (auto x : right) printf("%lld, ",x );
//         printf("\n");
    }
    printf("result: %lld\n", result);
}

int main()
{
    for (const auto& entry : fs::directory_iterator("adat"))
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




