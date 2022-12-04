
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

ll round_up_div(ll osztando, ll oszto)
{
    return (osztando + oszto - 1) / oszto;
}
ll gcd(ll a, ll b)
{
    while (b) b ^= a ^= b ^= a %= b;
    return a;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using cAbc = array<int, 3>;

array<cAbc, 14> abcs =
{ {
    { 1, 15, 13 },
    { 1, 10, 16 },
    { 1, 12, 2 },
    { 1, 10, 8 },
    { 1, 14, 11 },
    { 26, -11, 6 },
    { 1, 10, 12 },
    { 26, -16, 2 },
    { 26, -9, 2 },
    { 1, 11, 15 },
    { 26, -8, 1 },
    { 26, -8, 10 },
    { 26, -10, 14 },
    { 26, -9, 10 }
} };

int magic(int prev_z, int w, cAbc& abc)
{
    int a = abc[0], b = abc[1], c = abc[2];
    int x = prev_z % 26 + b;
    int z = prev_z / a;
    x = x != w;
    z *= 25 * x + 1;
    z += (w + c) * x;
    return z;
}

struct cNode
{
    bool initialized = false;
    int best = -1;
    int best_z;
    void initialize(int level, int prev_z);
};

struct cLevel
{
    int level;
    unordered_map<int, cNode> prev_zs;
    int getBest(int prev_z);
};

cLevel levels[14];

int cLevel::getBest(int prev_z)
{
    cNode& node = prev_zs[prev_z];
    if (!node.initialized)
    {
        node.initialized = true;
        node.initialize(level, prev_z);
    }
    return node.best;
}

void cNode::initialize(int level, int prev_z)
{
    for (int w = 1; w <= 9; ++w)
    {
        int w_to_z = magic(prev_z, w, abcs[level]);
        if ((level <13 && levels[level + 1].getBest(w_to_z) != -1)
            || ( level==13 && w_to_z == 0))
        {
            best = w;
            best_z = w_to_z;
            return;
        }
    }
}



void Solve()
{
    FOR(i, 14)
        levels[i].level = i;

    int best0 = levels[0].getBest(0);
    int prev_z = 0;
    FOR(i, 14)
    {
        cNode& node = levels[i].prev_zs[prev_z];
        printf("%d", node.best);
        prev_z = node.best_z;
    }
    printf("\nCheck: %d\n", prev_z);

//    magic(abcs[0]);
//    magic(-10, 26, 14);
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
        Solve();
//         printf("result: %lld\n", result);
        //         RI(verdict);
        //         if (verdict == -1)
        //             break;
    }
}






// void print(vector<int>& n)
// {
//     bool par = false;
//     for (auto x : n)
//     {
//         if (x == -1)
//             printf("[");
//     }
// }

// struct cNumber
// {
//     int value;
//     unique_ptr<cNumber> left, right;
//     bool is_real;
//     int depth;
// };
// 
// using up = unique_ptr<cNumber>;
// 
// const char* p;
// 
// unique_ptr<cNumber> read(int depth)
// {
//     up n = make_unique<cNumber>();
//     n->depth = depth;
//     if (*p == '[')
//     {
//         n->is_real = false;
//         ++p;
//         n->left = read(depth+1);
//         ++p; // ,
//         n->right = read(depth+1);
//         ++p; // ]
//     }
//     else
//     {
//         n->is_real = true;
//         n->value = *p - '0';
//         ++p;
//     }
//     return n;
// }
// 
// void reduce(cNumber& n)
// {
// 
// }