#define _CRT_SECURE_NO_WARNINGS
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
#include <ranges>
#include <print>
#include <bit>

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

namespace rng = std::ranges;
namespace views = std::ranges::views;
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
#define FOR(var, max_value) for(remove_reference<remove_const<decltype(max_value)>::type>::type var=0;var<max_value;++var)

constexpr ll mod = 1'000'000'007;
//constexpr ull mod = 998244353;
//        998244353
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define YES_NO_TASK

struct cPlace
{
    constexpr static int cop = 0;
    constexpr static int robber = 1;
    constexpr static int father = 2;
    constexpr static int mother = 3;

    constexpr static int boy = 0;
    constexpr static int girl = 1;

    array<int,4> adults; // cop, robber, father, mother
    array<int,2> children; // boy, girl

    bool isSafe() const
    {
        if (adults[robber] && !adults[cop])
        {
            int not_robber_count = adults[cop] + adults[father] + adults[mother] + children[boy] + children[girl];
            if (not_robber_count >= 1)
                return false;
        }
        if (children[boy] > 0 && adults[mother] && !adults[father])
            return false;
        if (children[girl] > 0 && adults[father] && !adults[mother])
            return false;
        return true;
    }
    cPlace& operator+=(const cPlace& other)
    {
        for (int i = 0; i < 4; ++i)
            adults[i] += other.adults[i];
        for (int i = 0; i < 2; ++i)
            children[i] += other.children[i];
        return *this;
    }
    void reset()
    {
        fill(ALL(adults), 0);
        fill(ALL(children), 0);
    }
};

struct cState
{
    cPlace from, to, boat;
    bool boat_on_from_side;
    bool isSafe() const
    {
        return from.isSafe() && to.isSafe() && boat.isSafe();
    }
    void unboard()
    {
        auto& place = boat_on_from_side ? from : to;
        place += boat;
        boat.reset();
    }
    bool isDone() const
    {
        return accumulate(ALL(to.adults), 0) == 4 && accumulate(ALL(to.children), 0) == 4;
    }
    int index() const
    {
        int idx = 0;
        for (int i = 0; i < 4; ++i)
        {
            idx |= from.adults[i] << i;
            idx |= to.adults[i] << (i + 4);
        }
        for(int i= 0; i < 2; ++i)
        {
            idx |= from.children[i] << (i * 2 + 8);
            idx |= to.children[i] << (i * 2 + 12);
        }
        idx |= boat_on_from_side ? 0 : (1 << 16);
        return idx;
    }
};

const char* adultNames[] = { "cop", "robber", "father", "mother" };
const char* childNames[] = { "boy", "girl" };

unordered_set<int> visited;

vector<string> solution;

bool step(cState state) // return true if success (done)
{
    if(visited.contains(state.index()))
        return false;
    visited.insert(state.index());
    auto& from = state.boat_on_from_side ? state.from : state.to;
    auto& to = state.boat_on_from_side ? state.to : state.from;
    auto& boat = state.boat;

    for (int ai1 = 0; ai1 < 4; ++ai1)
    {
        if (from.adults[ai1] == 0) continue;
        // take one adult
        {
            auto oldState = state;
            boat.adults[ai1]=1;
            from.adults[ai1]=0;
            if (state.isSafe())
            {
                state.boat_on_from_side = !state.boat_on_from_side;
                state.unboard();
                if (state.isSafe())
                {
                    if (state.isDone() || step(state))
                    {
                        solution.emplace_back(format("{} {}\n", state.boat_on_from_side ? "<-----" : "----->", adultNames[ai1]));
                        return true;
                    }
                }
            }
            state = oldState;
        }
        for (int ai2 = ai1 + 1; ai2 < 4; ++ai2)
        {
            if (from.adults[ai2] == 0) continue;
            // take 2 adults
            auto oldState = state;
            boat.adults[ai1]=1;
            boat.adults[ai2]=1;
            from.adults[ai1]=0;
            from.adults[ai2]=0;
            if (state.isSafe())
            {
                state.boat_on_from_side = !state.boat_on_from_side;
                state.unboard();
                if (state.isSafe())
                {
                    if (state.isDone() || step(state))
                    {
                        solution.emplace_back(format("{} {} and {}\n", state.boat_on_from_side ? "<-----" : "----->", adultNames[ai1], adultNames[ai2]));
                        return true;
                    }
                }
            }
            state = oldState;
        }
        for (int ci = 0; ci < 2; ++ci)
        {
            if (from.children[ci] == 0) continue;
            // take 1 adult + 1 child
            auto oldState = state;
            boat.adults[ai1]=1;
            boat.children[ci]=1;
            from.adults[ai1]=0;
            from.children[ci] -= 1;
            if (state.isSafe())
            {
                state.boat_on_from_side = !state.boat_on_from_side;
                state.unboard();
                if (state.isSafe())
                {
                    if (state.isDone() || step(state))
                    {
                        solution.emplace_back(format("{} {} and {}\n", state.boat_on_from_side ? "<-----" : "----->", adultNames[ai1], childNames[ci]));
                        return true;
                    }
                }
            }
            state = oldState;
        }
    }
    return false;
}

void Solve()
{
    cState state
    {
        .from = { .adults = {1,1,1,1}, .children={2,2} },
        .to = { .adults = {0,0,0,0}, .children={0,0} },
        .boat = { .adults = {0,0,0,0}, .children={0,0} },
        .boat_on_from_side = true
    };
    step(state);
    rng::reverse(solution);
    for (auto& s : solution)
        print("{}", s);
}

bool SolveYN()
{
    return true;
}

int Init()
{

    int t = 1;
    //RI(t);
    return t;
}

int main()
{
    int t = Init();
    for (int tc = 1; tc <= t; ++tc)
    {
        //    printf("Case #%d: ", tc);
#ifdef YES_NO_TASK
        printf("%s\n", SolveYN() ? "YES" : "NO");
#else
        Solve();
#endif
    }
}