
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

//constexpr int w = 139, h = 137;

void print(vector<char>& v, int w, int h)
{
    FOR(y, h)
    {
        printf("%.*s\n", w, v.data() + w * y);
    }
    printf("\n\n\n");
}

ll Solve()
{
    RII(w, h);
    vector<char> m(w * h + 1);
    vector<char> nm(w * h);
    for(int i=0; i<h; ++i)
    {
        scanf("%s", m.data() + w * i);
    }

    printf("Initial state:\n");
    print(m, w, h);
    for (int s = 0;; ++s)
    {
        bool wasChange = false;
        fill(ALL(nm), '.');
        char* p = m.data();
        char* dp = nm.data();
        for (int y = 0; y < h; ++y, ++p, ++dp)
        {
            for (int x = 0; x < w - 1; ++x, ++p, ++dp)
            {
                if (p[0] == '>')
                {
                    if (p[1] == '.')
                    {
                        dp[1] = '>';
                        wasChange = true;
                    }
                    else
                        dp[0] = '>';
                }
            }
            if (p[0] == '>')
            {
                if (p[1 - w] == '.')
                {
                    dp[1 - w] = '>';
                    wasChange = true;
                }
                else
                    dp[0] = '>';
            }
        }
        for (int x = 0; x < w; ++x)
        {
            p = m.data() + x;
            dp = nm.data() + x;
            for (int y = 0; y < h-1; ++y)
            {
                if (p[0] == 'v')
                {
                    if ((p[w] == '.' || p[w]=='>') && dp[w] == '.')
                    {
                        dp[w] = 'v';
                        wasChange = true;
                    }
                    else
                    {
                        dp[0] = 'v';
                    }
                }

                p += w;
                dp += w;
            }
            if (p[0] == 'v')
            {
                if ((m[x] == '.' || m[x] == '>') && nm[x] == '.')
                {
                    nm[x]='v';
                    wasChange=true;
                }
                else
                {
                    dp[0] = 'v';
                }
            }
        }
//         printf("After step %d\n", s + 1);
//         print(nm, w, h);

        if (!wasChange)
        {
            return s + 1;
        }
        swap(m, nm);
    }
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

//        printf("\n\n\ninner size: %lld\nall size: %lld\n\n", innerSize, allSize);
        //         RI(verdict);
        //         if (verdict == -1)
        //             break;
    }
}




