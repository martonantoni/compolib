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

struct cIntrusiveDisjointSetsItemBase
{
    size_t mParentIndex = -1;
    int mRank = 0;
    int mSize = 1;
    bool IsRoot() const { return mParentIndex == -1; }
    void SetJoined(cIntrusiveDisjointSetsItemBase& JoiningSet) { mSize += JoiningSet.mSize; }
};

template<class T> // T must have cIntrusiveDisjointSetsItemBase as parent class
class tIntrusiveDisjointSets
{
public:
    std::vector<T> mItems;
    size_t mNumberOfRoots = 0;
    void JoinIntoSet(size_t JoiningSet, size_t JoinedSet)
    {
        mItems[JoiningSet].mParentIndex = JoinedSet;
        // challenge specific code for union:
        mItems[JoinedSet].SetJoined(mItems[JoiningSet]);
        --mNumberOfRoots;
    }
    tIntrusiveDisjointSets() = default;
    tIntrusiveDisjointSets(tIntrusiveDisjointSets<T>& other) = delete;
    tIntrusiveDisjointSets& operator=(tIntrusiveDisjointSets<T>& other) = delete;
    tIntrusiveDisjointSets& operator=(tIntrusiveDisjointSets<T>&& other)
    {
        if (&other == this)
            return *this;
        mItems = std::move(other.mItems);
        mNumberOfRoots = other.mNumberOfRoots;
        other.mNumberOfRoots = 0;
        return *this;
    }
    void Init(size_t NumberOfItems)
    {
        mItems.resize(NumberOfItems);
        mNumberOfRoots = NumberOfItems;
    }
    void Init(std::vector<T>&& Items)
    {
        mItems = std::move(Items);
        mNumberOfRoots = mItems.size();
    }
    int AddItem()
    {
        mItems.emplace_back();
        ++mNumberOfRoots;
        return (int)mItems.size() - 1;
    }
    template<class U> int AddItem(U&& item)
    {
        mItems.emplace_back(std::forward<U>(item));
        ++mNumberOfRoots;
        return (int)mItems.size() - 1;
    }
    size_t GetRoot(size_t i) const
    {
        auto& Item = mItems[i];
        if (Item.mParentIndex == -1)
            return i;
        // path compression:
        auto RootIndex = GetRoot(Item.mParentIndex);
        const_cast<size_t&>(Item.mParentIndex) = RootIndex;
        return RootIndex;
    }
    void Union(size_t a, size_t b)
    {
        auto a_root_i = GetRoot(a), b_root_i = GetRoot(b);
        if (a_root_i == b_root_i)
            return;
        if (mItems[a_root_i].mRank > mItems[b_root_i].mRank)
        {
            JoinIntoSet(b_root_i, a_root_i);
        }
        else
        {
            JoinIntoSet(a_root_i, b_root_i);
            if (mItems[a_root_i].mRank == mItems[b_root_i].mRank)
                ++mItems[b_root_i].mRank;
        }
    }
    template<class F>
    void ForEachRoot(const F& f)
    {
        for (auto& Item : mItems)
            if (Item.IsRoot())
                f(Item);
    }
    size_t GetRootCount() const { return mNumberOfRoots; }
    T& operator[](size_t i) { return mItems[i]; }
    const T& operator[](size_t i) const { return mItems[i]; }
    auto begin() { return mItems.begin(); }
    auto end() { return mItems.end(); }
    void clear() { mItems.clear(); mNumberOfRoots = 0; }
};


struct cCube : public cIntrusiveDisjointSetsItemBase
{
    int x, y, z;
    int free_sides = 6;
    int area = 0;
};

tIntrusiveDisjointSets<cCube> sets;

array<int, 32 * 32 * 32> space;

int& at(int x, int y, int z)
{
    return space[x + y * 32 + z * 32 * 32];
}


static constexpr int line_count = 2825;
//static constexpr int line_count = 13;

void prepare()
{
    sets.Init(line_count);
    cFastFileReader in("in.txt");
    fill(ALL(space), -1);
    int cube_i = 0;
    int free_sides = 0;
    for (auto line : in)
    {
        free_sides += 6;
        auto coords = line.toStringVector(",").ToIntVector();
        int x = (int)coords[0] + 1;
        int y = (int)coords[1] + 1;
        int z = (int)coords[2] + 1;
        sets[cube_i].x = x;
        sets[cube_i].y = y;
        sets[cube_i].z = z;
        auto check = [&](int xoffset, int yoffset, int zoffset)
        {
            int neighbour = at(x + xoffset, y + yoffset, z + zoffset);
            if (neighbour != -1)
            {
                sets.Union(neighbour, cube_i);
                --sets[cube_i].free_sides;
                --sets[neighbour].free_sides;

                free_sides -= 2;
            }
        };
        check(1, 0, 0);
        check(-1, 0, 0);
        check(0, 1, 0);
        check(0, -1, 0);
        check(0, 0, 1);
        check(0, 0, -1);
        at(x, y, z) = cube_i;
        ++cube_i;
    }
    for (int i = 0; i < line_count;++i)
    {
        sets[sets.GetRoot(i)].area += sets[i].free_sides;
    }
    printf("Free sides: %d\n", free_sides);
}

// void Solve(ll r)
// {
// 
// }
// 
void Solve()
{
}

void Solve2()
{
    sets.ForEachRoot([](cCube& cube)
        {
            printf("volume: %d, area: %d\n", cube.mSize, cube.area);
        });


    ll result = 0;

    vector<tuple<int, int, int>> f;
    f.emplace_back(0, 0, 0);

    while (!f.empty())
    {
        auto [x, y, z] = f.back();
        printf("checking %d, %d, %d\n", x, y, z);
        f.pop_back();
        if (at(x, y, z) == -1)
        {
            auto check = [&](int xo, int yo, int zo)
            {
                int xx = x + xo, yy = y + yo, zz = z + zo;
                if (xx >= 0 && xx < 30 && yy >= 0 && yy < 30 && zz >= 0 && zz < 30)
                {
                    printf("at %d, %d, %d: %d\n", xx, yy, zz, at(xx, yy, zz));
                    if (at(xx, yy, zz) == -1)
                    {
                        f.emplace_back(xx, yy, zz);
                    }
                    else if(at(xx, yy, zz)>=0)
                    {
                        ++result;
                    }
                }
            };
            check(1, 0, 0);
            check(-1, 0, 0);
            check(0, 1, 0);
            check(0, -1, 0);
            check(0, 0, 1);
            check(0, 0, -1);
            at(x, y, z) = -2;
        }
    }



//     for (int x = 0; x < 30; ++x)
//     {
//         for (int y = 0; y < 30; ++y)
//         {
//             for (int z = 0; z < 30; ++z)
//             {
//                 if (at(x, y, z) != -1)
//                 {
//                     result += 2;
//                     break;
//                 }
//             }
//         }
//     }
// 
//     for (int x = 0; x < 30; ++x)
//     {
//         for (int y = 0; y < 30; ++y)
//         {
//             for (int z = 0; z < 30; ++z)
//             {
//                 if (at(x, z, y) != -1)
//                 {
//                     result += 2;
//                     break;
//                 }
//             }
//         }
//     }
// 
//     for (int x = 0; x < 30; ++x)
//     {
//         for (int y = 0; y < 30; ++y)
//         {
//             for (int z = 0; z < 30; ++z)
//             {
//                 if (at(z, x, y) != -1)
//                 {
//                     result += 2;
//                     break;
//                 }
//             }
//         }
//     }

    // 2054 too low

    printf("outer: %lld\n", result);

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
