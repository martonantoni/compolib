
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr int colA = 1;
constexpr int colB = 2;
constexpr int colC = 3;
constexpr int colD = 4;

constexpr ull stepCosts[5] = { 0, 1, 10, 100, 1000 };

constexpr ull b(int p) { return 7ull << (p*3); }
constexpr ull bits() { return 0ull; }
template<class... Ts> constexpr ull bits(int p, Ts... ps)
{
    return b(p)| bits(ps...);
}

void setAt(ull& state, int p, int c)
{
    state &= ~b(p);
    state |= static_cast<ull>(c) << (p * 3);
}
constexpr int getAt(ull state, int p)
{
    return static_cast<int>( (state & b(p)) >> (p * 3) );
}
int getHomeQueueLength(ull state, int homeIndex)
{
    return getAt(state, 7 + homeIndex);
}
void setHomeQueueLength(ull& state, int homeIndex, int size)
{
    setAt(state, 7 + homeIndex, size);
}
bool isHomeFree(ull state, int homeIndex)
{
    return getHomeQueueLength(state, homeIndex) == 0;
}

using cRoute = array<pair<ull, int>, 7>; // bits, len
using cAllRoutes = array<cRoute, 4>;

constexpr cAllRoutes allRouteHomes =
{ {
    {{
        make_pair<ull, int>(bits(1            ), 3), // 0
        make_pair<ull, int>(bits(             ), 2), // 1
        make_pair<ull, int>(bits(             ), 2), // 2
        make_pair<ull, int>(bits(   2         ), 4), // 3
        make_pair<ull, int>(bits(   2, 3      ), 6), // 4
        make_pair<ull, int>(bits(   2, 3, 4   ), 8), // 5
        make_pair<ull, int>(bits(   2, 3, 4, 5), 9)  // 6
    }},
    {{
        make_pair<ull, int>(bits(1, 2          ), 5), // 0
        make_pair<ull, int>(bits(   2          ), 4), // 1
        make_pair<ull, int>(bits(              ), 2), // 2
        make_pair<ull, int>(bits(              ), 2), // 3
        make_pair<ull, int>(bits(       3      ), 4), // 4
        make_pair<ull, int>(bits(       3, 4   ), 6), // 5
        make_pair<ull, int>(bits(       3, 4, 5), 7), // 6
    }},
    {{
        make_pair<ull, int>(bits(1, 2, 3     ), 7), // 0
        make_pair<ull, int>(bits(   2, 3     ), 6), // 1
        make_pair<ull, int>(bits(      3     ), 4), // 2
        make_pair<ull, int>(bits(            ), 2), // 3
        make_pair<ull, int>(bits(            ), 2), // 4
        make_pair<ull, int>(bits(        4   ), 4), // 5
        make_pair<ull, int>(bits(        4, 5), 5), // 6
    }},
    {{
        make_pair<ull, int>(bits(1, 2, 3, 4), 9),  // 0
        make_pair<ull, int>(bits(   2, 3, 4), 8),  // 1
        make_pair<ull, int>(bits(      3, 4), 6),  // 2
        make_pair<ull, int>(bits(         4), 4),  // 3
        make_pair<ull, int>(bits(          ), 2),  // 4
        make_pair<ull, int>(bits(          ), 2),  // 5
        make_pair<ull, int>(bits(         5), 3),  // 6
    }}

} };


int checkRouteHome(ull state, int p, int homeIndex)
{
    auto [routeBits, length] = allRouteHomes[homeIndex][p];
    return (state & routeBits) ? 0 : length;
}

constexpr cAllRoutes allRouteOutside =
{ {
    {{
        make_pair<ull, int>(bits(0, 1),                 3),  // 0
        make_pair<ull, int>(bits(   1),                 2),  // 1
        make_pair<ull, int>(bits(       2),             2),  // 2
        make_pair<ull, int>(bits(       2, 3),          4),  // 3
        make_pair<ull, int>(bits(       2, 3, 4),       6),  // 4
        make_pair<ull, int>(bits(       2, 3, 4, 5),    8),  // 5
        make_pair<ull, int>(bits(       2, 3, 4, 5, 6), 9)   // 6
    }},
    {{
        make_pair<ull, int>(bits(0, 1, 2),              5),  // 0
        make_pair<ull, int>(bits(   1, 2),              4),  // 1
        make_pair<ull, int>(bits(      2),              2),  // 2
        make_pair<ull, int>(bits(          3),          2),  // 3
        make_pair<ull, int>(bits(          3, 4),       4),  // 4
        make_pair<ull, int>(bits(          3, 4, 5)   , 6),  // 5
        make_pair<ull, int>(bits(          3, 4, 5, 6), 7),  // 6
    }},
    {{
        make_pair<ull, int>(bits(0, 1, 2, 3),        7),  // 0
        make_pair<ull, int>(bits(   1, 2, 3),        6),  // 1
        make_pair<ull, int>(bits(      2, 3),        4),  // 2
        make_pair<ull, int>(bits(         3),        2),  // 3
        make_pair<ull, int>(bits(          4),       2),  // 4
        make_pair<ull, int>(bits(          4, 5),    4),  // 5
        make_pair<ull, int>(bits(          4, 5, 6), 5),  // 6
    }},
    {{
        make_pair<ull, int>(bits(0, 1, 2, 3, 4)       , 9), // 0
        make_pair<ull, int>(bits(   1, 2, 3, 4)       , 8), // 1
        make_pair<ull, int>(bits(      2, 3, 4)       , 6), // 2
        make_pair<ull, int>(bits(         3, 4)       , 4), // 3
        make_pair<ull, int>(bits(            4)       , 2), // 4
        make_pair<ull, int>(bits(                5)   , 2), // 5
        make_pair<ull, int>(bits(                5, 6), 3), // 6
    }}

} };


int checkRouteOutside(ull state, int target, int homeIndex)
{
    auto [routeBits, length] = allRouteOutside[homeIndex][target];
    return (state & routeBits) ? 0 : length;
}

vector<pair<ull, ull>> states; // cost, state

unordered_map<ull, ull> bestMap;

constexpr int initalHomeQueueSizes = 4;

using cHomeQueue = array<int, initalHomeQueueSizes>;
using cHomeQueues = array<cHomeQueue, 4>;

#define initalHomeQueueSizes 4
//#define USE_SAMPLE

#if initalHomeQueueSizes == 4
#ifndef USE_SAMPLE
// MY: 
cHomeQueues homeQueues =
{ {
    { colD, colD, colD, colB },
    { colC, colC, colB, colC },
    { colA, colB, colA, colD },
    { colB, colA, colC, colA},
} };

#else
// SAMPLE

cHomeQueues homeQueues =
{ {
       // { colA, colA, colA, colD },



{ colB, colD, colD, colA },

{ colC, colC, colB, colD },
//{ colB, colB, colB, colA },


{ colB, colB, colA, colC },
//  { colC, colC, colC, colA },


{ colD, colA, colC, colA},


} };

#endif
#else
#ifndef USE_SAMPLE

// MY part 1:

cHomeQueues homeQueues =
{ {
    { colD,  colB },
    { colC,  colC },
    { colA,  colD },
    { colB,  colA},
} };

#else
// SAMPLE part 1:

cHomeQueues homeQueues =
{ {
    { colB,  colA },
    { colC,  colD },
    { colB,  colC },
    { colD,  colA},
} };

#endif
#endif

void printState(ull state)
{
    const char* abcd = ".ABCD";

    int found[5] = { 0,0,0,0,0 };
    FOR(p, 7)
        ++found[getAt(state, p)];
    FOR(h, 4)
    {
        FOR(qi, getHomeQueueLength(state, h))
        {
            ++found[homeQueues[h][3 - qi]];
        }
    }

    printf("#############\n"
           "#%c%c.%c.%c.%c.%c%c#\n"
           "###%c#%c#%c#%c###\n"
           "  #%c#%c#%c#%c#\n"
           "  #%c#%c#%c#%c#\n"
           "  #%c#%c#%c#%c#\n"
           "  #########\n",
        abcd[getAt(state, 0)],
        abcd[getAt(state, 1)],
        abcd[getAt(state, 2)],
        abcd[getAt(state, 3)],
        abcd[getAt(state, 4)],
        abcd[getAt(state, 5)],
        abcd[getAt(state, 6)],

        getHomeQueueLength(state, 0) >= 4 ? abcd[homeQueues[0][0]] : (found[colA] <= 0 ? 'A' : '.'),
        getHomeQueueLength(state, 1) >= 4 ? abcd[homeQueues[1][0]] : (found[colB] <= 0 ? 'B' : '.'),
        getHomeQueueLength(state, 2) >= 4 ? abcd[homeQueues[2][0]] : (found[colC] <= 0 ? 'C' : '.'),
        getHomeQueueLength(state, 3) >= 4 ? abcd[homeQueues[3][0]] : (found[colD] <= 0 ? 'D' : '.'),

        getHomeQueueLength(state, 0) >= 3 ? abcd[homeQueues[0][1]] : (found[colA] <= 1 ? 'A' : '.'),
        getHomeQueueLength(state, 1) >= 3 ? abcd[homeQueues[1][1]] : (found[colB] <= 1 ? 'B' : '.'),
        getHomeQueueLength(state, 2) >= 3 ? abcd[homeQueues[2][1]] : (found[colC] <= 1 ? 'C' : '.'),
        getHomeQueueLength(state, 3) >= 3 ? abcd[homeQueues[3][1]] : (found[colD] <= 1 ? 'D' : '.'),

        getHomeQueueLength(state, 0) >= 2 ? abcd[homeQueues[0][2]] : (found[colA] <= 2 ? 'A' : '.'),
        getHomeQueueLength(state, 1) >= 2 ? abcd[homeQueues[1][2]] : (found[colB] <= 2 ? 'B' : '.'),
        getHomeQueueLength(state, 2) >= 2 ? abcd[homeQueues[2][2]] : (found[colC] <= 2 ? 'C' : '.'),
        getHomeQueueLength(state, 3) >= 2 ? abcd[homeQueues[3][2]] : (found[colD] <= 2 ? 'D' : '.'),

        getHomeQueueLength(state, 0) >= 1 ? abcd[homeQueues[0][3]] : (found[colA] <= 3 ? 'A' : '.'),
        getHomeQueueLength(state, 1) >= 1 ? abcd[homeQueues[1][3]] : (found[colB] <= 3 ? 'B' : '.'),
        getHomeQueueLength(state, 2) >= 1 ? abcd[homeQueues[2][3]] : (found[colC] <= 3 ? 'C' : '.'),
        getHomeQueueLength(state, 3) >= 1 ? abcd[homeQueues[3][3]] : (found[colD] <= 3 ? 'D' : '.'));
}

unordered_map<ull, ull> prev_states;

void pushState(ull cost, ull state, ull prev_state)
{
    ull& bestForState = bestMap[state];
    if (bestForState > 0 && cost >= bestForState)
        return;
    bestForState = cost;
    prev_states[state] = prev_state;

    if (state)
    {
        states.emplace_back(cost, state);
        push_heap(ALL(states), greater<>());
    }
}

void Solve()
{
    ull initial_state = 0;
//    setAt(initial_state, 6, colB);

    setHomeQueueLength(initial_state, 0, initalHomeQueueSizes);
   setHomeQueueLength(initial_state, 1, initalHomeQueueSizes);
    setHomeQueueLength(initial_state, 2, initalHomeQueueSizes);
    setHomeQueueLength(initial_state, 3, initalHomeQueueSizes);

//    setHomeQueueLength(initial_state, 1, 4);
//     setAt(initial_state, 0, colD);
//     setAt(initial_state, 5, colD);
//     setAt(initial_state, 6, colD);

    ull best = numeric_limits<ull>::max();
    states.emplace_back(0, initial_state);
    for (int s=0;!states.empty();++s)
    {
        const auto [cost, state] = states.front();
        if (cost >= best)
            break;
        pop_heap(ALL(states), greater<>());
        states.pop_back();

//         ull& bestForState = bestMap[state];
//         if (bestForState!=0&&bestForState<=cost)
//             continue;
//         bestForState = cost;

        for (int start_pos = 0; start_pos <= 6; ++start_pos)
        {
            int color = getAt(state, start_pos);
            if(!color)
                continue;
            int homeIndex = color - 1;
            if (isHomeFree(state, homeIndex))
            {
                int stepsNeeded = checkRouteHome(state, start_pos, homeIndex);
                if (stepsNeeded)
                {
                    ull newState = state;
                    setAt(newState, start_pos, 0);

//////////////////////////////////////////////////////////////////////////

                    int found[5] = { 0,0,0,0,0 };
                    FOR(p, 7)
                        ++found[getAt(state, p)];
                    FOR(h, 4)
                    {
                        FOR(qi, getHomeQueueLength(state, h))
                        {
                            ++found[homeQueues[h][3 - qi]];
                        }
                    }
                    stepsNeeded += found[color] - 1;


//////////////////////////////////////////////////////////////////////////
                    ull newCost = cost + stepsNeeded * stepCosts[color];
#ifdef DEBUG
                    {
                        printf("%c from %d going home (steps: %d, totalCost: %llu):\n",
                            " ABCD"[color], start_pos, stepsNeeded, newCost);
                        printState(state);
                    }
#endif

                    pushState(newCost, newState, state);
                    if (newState == 0)
                    {
                        best = min(best, newCost);
                    }
                }
            }
        }
        for (int homeIndex = 0; homeIndex < 4; ++homeIndex)
        {
            int queueLength = getHomeQueueLength(state, homeIndex);
            if (queueLength)
            {
                int color = homeQueues[homeIndex][initalHomeQueueSizes - queueLength];
                // sample fix:
#ifdef USE_SAMPLE
                if (color - 1 == homeIndex && queueLength == 1)
                {
                    ull newState = state;
                    setHomeQueueLength(newState, homeIndex, 0);
                    pushState(cost, newState, state);
                    if (newState == 0)
                    {
                        best = min(best, cost);
                    }
                    continue;
                }
#endif
                for (int target_pos = 0; target_pos <= 6; ++target_pos)
                {
                    int steps = checkRouteOutside(state, target_pos, homeIndex);
                    if (steps)
                    {
                        ull newState = state;
                        setHomeQueueLength(newState, homeIndex, queueLength - 1);
                        setAt(newState, target_pos, color);
                        steps += initalHomeQueueSizes - queueLength;
                        ull newCost = cost + steps * stepCosts[color];

                        pushState(newCost, newState, state);
                    }
                }
            }
        }
    }

    vector<ull> solution_states;
    ull ss = 0;
    do 
    {
        solution_states.emplace_back(ss);
        ss = prev_states[ss];
    } while (ss != initial_state);
    solution_states.emplace_back(initial_state);
    reverse(ALL(solution_states));
    printf("SOLUTION:\n");
    for (ull s : solution_states)
    {
        printState(s);
        printf("cost: %llu\n", bestMap[s]);
        printf("\n\n");
    }

    ull extra = (initalHomeQueueSizes - 1) * 1111;
#ifdef USE_SAMPLE
#if initalHomeQueueSizes == 4
    extra -= 303;
#else
    extra -= 101;
#endif
#endif
//    printf("best cost: %llu   (raw: %llu)\n", best + extra, best);
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
 //       printf("result: %lld\n", result);

//        printf("\n\n\ninner size: %lld\nall size: %lld\n\n", innerSize, allSize);
        //         RI(verdict);
        //         if (verdict == -1)
        //             break;
    }
}
