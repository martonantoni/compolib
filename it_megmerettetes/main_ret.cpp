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




char buf[10'000'000];


// struct cPos
// {
//     int r, c;
//     int cost;
// };
 

void Solve(FILE* f)
{
    vector<string> rs;
    for (;;)
    {
        if (fscanf(f, "%s", buf) <= 0)
            break;
        rs.emplace_back(buf);
    }
    auto at = [&](int r, int c) -> char&
    {
        return rs[r][c];
    };
    int rows = (int)rs.size(), cols = (int)rs[0].length();
    vector<pair<int, pair<int, int>>> fr;  // cost, pos
    fr.emplace_back(0, make_pair( 0, 0 ));
    for (;;)
    {
        auto pos = fr.front().second;
        auto cost = fr.front().first;
        pop_heap(ALL(fr), greater<>());
        fr.pop_back();
        if (pos.first == rows - 1 && pos.second == cols - 1)
        {
            printf("COST: %d\n", cost);
            return;
        }
        char& cc = at(pos.first, pos.second);
        if(cc=='x')
            continue;
        int step_count = pos.first + pos.second + 1;
        if (pos.first + 1 < rows)
        {
            int step_cost = at(pos.first + 1, pos.second) == cc ? 0 : step_count;
            fr.emplace_back(cost + step_cost, make_pair(pos.first + 1, pos.second));
            push_heap(ALL(fr), greater<>());
        }
        if (pos.second + 1 < cols)
        {
            int step_cost = at(pos.first, pos.second + 1) == cc ? 0 : step_count;
            fr.emplace_back(cost + step_cost, make_pair(pos.first, pos.second + 1));
            push_heap(ALL(fr), greater<>());
        }
        cc = 'x';
    }
}


int main()
{
    vector<string> tasks;
    unordered_map<string, string> solutions;
    for (const auto& entry : fs::directory_iterator("ret"))  // <<<<--------------------------------------- folder name
    {
        if (fs::is_regular_file(entry) && entry.path().extension() == ".txt")
        {
            string file_name = entry.path().string();
            if (file_name.substr(file_name.length() - 8, 8) == ".out.txt")
            {
                FILE* f = fopen(file_name.c_str(), "r");
                string content(entry.file_size() + 1, 0);
                fread(content.data(), 1, entry.file_size(), f);
                fclose(f);
                file_name.erase(file_name.begin() + file_name.length() - 8, file_name.end());
                file_name += ".in.txt";
                solutions[file_name] = content;
            }
            else
            {
                tasks.emplace_back(file_name);
            }
        }
    }
    for (auto& file_name : tasks)
    {
        printf("\n--------------------------------------------------------\n-=#=- %s -=#=-\n", file_name.c_str());
        FILE* f = fopen(file_name.c_str(), "r");
        Solve(f);
        auto i = solutions.find(file_name);
        if (i != solutions.end())
        {
            printf("!!! OFFICIAL SOLUTION:\n%s\n", i->second.c_str());
        }
        fclose(f);
        fflush(stdout);

    }


    return 0;
}




