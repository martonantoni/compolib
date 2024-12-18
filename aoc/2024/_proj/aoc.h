#pragma once

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
#include <queue>
#include <array>
#include <map>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <cstddef>
#include <cinttypes>
#include <regex>
#include <filesystem>
#include <ctime>
#include <ranges>
#ifdef __cpp_lib_print
#include <print>
#endif

using ll = long long;
using cIntVector = std::vector<ll>;

#include "string_vector.h"
#include "clipboard.h"

namespace fs = std::filesystem;
namespace rng = std::ranges;
namespace vs = std::views;

using std::views::enumerate;
using std::views::iota;
using std::views::zip;

#undef max
#undef min
#undef I

#ifdef __GNUC__ 
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

extern FILE* out;

#ifdef __cpp_lib_print
#define P(...) do { if(out) { std::print(out, "\n{}", print_prefix); print(out, __VA_ARGS__); } } while(false)
#define PC(...) do { if(out) { std::print(out, __VA_ARGS__); } } while(false)
#define RESULT(arg) do { if(out) { std::print(out, "\n{}RESULT: {}", print_prefix, arg); } textToClipboard(to_string(arg)); } while(false)
#else
#define P(...)
#define PC(...)
#define RESULT(arg) do { std::cout << "\n" << print_prefix << "RESULT: " << arg << std::endl; textToClipboard(to_string(arg)); } while(false)
#endif


#ifdef _DEBUG
#define ASSERT(x) if(!(x)) __debugbreak()
#define D(expression) expression
#else
//char* crash_please = (char*)42;
#define ASSERT(x) 
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
        printf("\n--- Elapsed %llu.%llu ms in %s ---\n\n", (unsigned long long)Elapsed.count() / 1000, (unsigned long long)Elapsed.count() % 1000, mName);
    }
    void waypoint()
    {
        auto EndTime = std::chrono::high_resolution_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - mStartTime);
        printf("+ %llu.%llu ms: ", (unsigned long long)Elapsed.count() / 1000, (unsigned long long)Elapsed.count() % 1000);
    }
};

using namespace std;
using namespace std::chrono;
using namespace std::string_literals;
using ull = unsigned long long;
using cIntVector = vector<ll>;

template<class T> using min_heap = priority_queue<T, vector<T>, greater<T>>;
template<class T> using max_heap = priority_queue<T>;

#define ALL(cont) cont.begin(), cont.end()
#define FOR(var, max_value) for(remove_reference<remove_const<decltype(max_value)>::type>::type var=0;var<max_value;++var)

constexpr ll mod = 1'000'000'007;

struct cLine
{
    vector<ll> i;
    cStringVector s;
    string txt;
    bool is_empty = false;
    int idx;
};

extern vector<cLine> ls;
extern vector<vector<cLine>> blocks;

extern const char* main_delimeters;
extern bool main_allow_empty_fields;
extern bool is_first_part;
extern bool is_example;
extern const char* print_prefix;

extern const char* outFileName;
extern const char* inFileName;
extern const char* exampleFileName;

void solve(bool first);

void solveFirst();
void solveSecond();




// Hivasnal nagyon figyelni, hogy a "to" az mar nem lehetseges ertek kell hogy legyen

#define higher true
#define lower false
auto value_lower_bound(auto from, auto to, auto p)
{
    for (;;)
    {
        if (from == to)
            return from - 1;
        auto mid = from + (to - from) / 2;
        bool result = p(mid);
#ifdef _DEBUG
        printf("trying %lld, result: %s\n", mid, result == higher ? "higher" : "lower");
#endif
        if (result == higher)
        {
            from = mid + 1;
        }
        else
        {
            to = mid;
        }
    }
}
