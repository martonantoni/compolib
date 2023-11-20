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

/*
struct cGraphEdge
{
    ll to;
    ll weight;
    cGraphEdge(ll t, ll w) : to(t), weight(w) {}
};

struct cNode
{
    bool is_connected = false;
    vector<cGraphEdge> edges;
    vector<cGraphEdge> tree_edges;
};


void Solve(FILE* f)
{
    vector<cNode> graph_nodes;
    vector<pair<cGraphEdge, ll>> frontier;

    RII(n, m);

    ll total_w = 0;
    

    graph_nodes.resize(n);
    for (ll i = 0; i < m; ++i)
    {
        RLLL(from, to, w);
        graph_nodes[from].edges.emplace_back(to, w);
        graph_nodes[to].edges.emplace_back(from, w);
    }
    int connected_nodes = 1;
    frontier.reserve(graph_nodes[0].edges.size());
    for (auto& e : graph_nodes[0].edges)
        frontier.emplace_back(e, 0);
    sort(frontier.begin(), frontier.end(), [](auto& a, auto& b) { return a.first.weight > b.first.weight; });
    graph_nodes[0].is_connected = true;
    while (connected_nodes < n)
    {
        // pop redundant edges:
        while (graph_nodes[frontier.back().first.to].is_connected)
            frontier.pop_back();

        ll new_node = frontier.back().first.to;
        ll from = frontier.back().second;
        graph_nodes[from].tree_edges.emplace_back(frontier.back().first);
        total_w += frontier.back().first.weight;
        graph_nodes[new_node].is_connected = true;
        // add new edges to frontier

   	frontier.erase(remove_if(frontier.begin(), frontier.end(), [new_node](auto &e) { return e.first.to==new_node; }), frontier.end());

        for (auto& e : graph_nodes[new_node].edges)
        {
            if (!graph_nodes[e.to].is_connected)
            {
                auto i = lower_bound(frontier.begin(), frontier.end(), e.weight, [](auto& e, ll w) { return e.first.weight > w; });
                //makes it slower:				if(none_of(i, frontier.end(), [to=e.to](auto &e) { return e.first.to==to; }))
                frontier.emplace(i, e, new_node);
            }
        }
        ++connected_nodes;
    }

    printf("total w: %lld\n", total_w);

}*/

#define M_PI           3.14159265358979323846

double direction(pair<ll, ll> f, pair<ll, ll> t)
{
    // Calculate the differences in x and y coordinates
    long long deltaX = t.first - f.first;
    long long deltaY = t.second - f.second;

    // Calculate the angle using arctan (atan2 is used to handle all cases)
    double angle = atan2(deltaY, deltaX);

    return angle;
}

bool areAnglesOpposite(double angle1, double angle2) {
    // Check if the sum of angles is equal to pi (180 degrees)
    return fabs(angle1 + angle2 - M_PI) < 1e-9;
}

// Function to calculate the cross product of two vectors formed by points p1, p2, and p3
long long crossProduct(const pair<long long, long long>& p1, const pair<long long, long long>& p2, const pair<long long, long long>& p3) {
    return (p2.first - p1.first) * (p3.second - p1.second) - (p3.first - p1.first) * (p2.second - p1.second);
}

// Function to check if any three points are collinear
bool arePointsCollinear(const pair<long long, long long>& p1, const pair<long long, long long>& p2, const pair<long long, long long>& p3, const pair<long long, long long>& p4) {
    return crossProduct(p1, p2, p3) == 0 || crossProduct(p1, p2, p4) == 0 || crossProduct(p1, p3, p4) == 0 || crossProduct(p2, p3, p4) == 0;
}

// Function to calculate the square of the Euclidean distance between two points
long long distanceSquared(const pair<long long, long long>& p1, const pair<long long, long long>& p2) {
    return (p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second);
}

// Function to check if the four points form a square
bool arePointsFormingSquare(const pair<long long, long long>& p1, const pair<long long, long long>& p2,
    const pair<long long, long long>& p3, const pair<long long, long long>& p4) {
    // Calculate distances between all pairs of points
    long long d1 = distanceSquared(p1, p2);
    long long d2 = distanceSquared(p1, p3);
    long long d3 = distanceSquared(p1, p4);
    long long d4 = distanceSquared(p2, p3);
    long long d5 = distanceSquared(p2, p4);
    long long d6 = distanceSquared(p3, p4);

    // Put distances in an array for easy comparison
    long long distances[] = { d1, d2, d3, d4, d5, d6 };
    sort(distances, distances + 6);

    // Check if the distances are consistent with a square
    return distances[0] > 0 && distances[0] == distances[1] && distances[1] == distances[2] && distances[2] == distances[3]
        && distances[4] == distances[5] && 2 * distances[0] == distances[4];
}


// Function to check if two vectors are perpendicular
bool areVectorsPerpendicular(const pair<long long, long long>& v1, const pair<long long, long long>& v2) {
    long long dotProduct = v1.first * v2.first + v1.second * v2.second;
    return dotProduct == 0;
}

// Function to check if two vectors are parallel
bool areVectorsParallel(const pair<long long, long long>& v1, const pair<long long, long long>& v2) {
    return v1.first * v2.second - v1.second * v2.first == 0;
}

pair<ll, ll> vec(const pair<ll, ll> p1, const pair<ll, ll> p2)
{
    return { p2.first - p1.first, p2.second - p1.second };
}


// Function to check if the four points form a rectangle
bool arePointsFormingRectangle(const pair<long long, long long>& p1, const pair<long long, long long>& p2,
    const pair<long long, long long>& p3, const pair<long long, long long>& p4) {
    // Calculate distances between all pairs of points
   // Calculate distances between all pairs of points
    long long d1 = distanceSquared(p1, p2);
    long long d2 = distanceSquared(p2, p3);
    long long d3 = distanceSquared(p3, p4);
    long long d4 = distanceSquared(p4, p1);
    long long diagonal1 = distanceSquared(p1, p3);
    long long diagonal2 = distanceSquared(p2, p4);

    if ((d1 == d3 && d2 == d4) || (d1 == d2 && d3 == d4 && diagonal1 == diagonal2))
    {
        if (areVectorsPerpendicular(vec(p1, p2), vec(p2, p3)))
            return true;
    }
    return false;
}

// Function to check if the four points form a rectangle
bool arePointsFormingRhomboid(const pair<long long, long long>& p1, const pair<long long, long long>& p2,
    const pair<long long, long long>& p3, const pair<long long, long long>& p4) {
    // Calculate distances between all pairs of points
   // Calculate distances between all pairs of points
    long long d1 = distanceSquared(p1, p2);
    long long d2 = distanceSquared(p2, p3);
    long long d3 = distanceSquared(p3, p4);
    long long d4 = distanceSquared(p4, p1);
    long long diagonal1 = distanceSquared(p1, p3);
    long long diagonal2 = distanceSquared(p2, p4);

    if ((d1 == d3 && d2 == d4) || (d1 == d2 && d3 == d4 && diagonal1 == diagonal2))
    {
        if (areVectorsPerpendicular(vec(p1, p2), vec(p2, p3)))
            return true;
    }
    return false;
}


// Function to check if two line segments intersect
bool doIntersect(const pair<long long, long long>& p1, const pair<long long, long long>& q1,
    const pair<long long, long long>& p2, const pair<long long, long long>& q2) {
    long long cp1 = crossProduct(p1, q1, p2);
    long long cp2 = crossProduct(p1, q1, q2);
    long long cp3 = crossProduct(p2, q2, p1);
    long long cp4 = crossProduct(p2, q2, q1);

    // If the endpoints of one segment are on opposite sides of the other segment, they intersect
    return ((cp1 > 0 && cp2 < 0) || (cp1 < 0 && cp2 > 0)) && ((cp3 > 0 && cp4 < 0) || (cp3 < 0 && cp4 > 0));
}

// Function to check if there are two edges in the square that cross each other
bool areEdgesCrossing(const pair<long long, long long>& p1, const pair<long long, long long>& p2,
    const pair<long long, long long>& p3, const pair<long long, long long>& p4) {
    // Check if any pair of edges intersect
    return doIntersect(p1, p2, p3, p4) || doIntersect(p2, p3, p4, p1);
}


#define NEAR(a, b) (abs(a-b)<0.0000001)

char check(vector<pair<ll, ll>> s)
{
    if (s[0] == s[1] || s[0] == s[2] || s[0] == s[3] || s[1] == s[2] || s[1] == s[3] || s[2] == s[3])
        return 'E';
    double dir[4];
    dir[0] = direction(s[0], s[1]);
    dir[1] = direction(s[1], s[2]);
    dir[2] = direction(s[2], s[3]);
    dir[3] = direction(s[3], s[0]);
    double dir_op[4];
    dir_op[0] = direction(s[1], s[0]);
    dir_op[1] = direction(s[2], s[1]);
    dir_op[2] = direction(s[3], s[2]);
    dir_op[3] = direction(s[0], s[3]);
//     if(NEAR(dir[0], dir[1])|| NEAR(dir[1], dir[2]) || NEAR(dir[2], dir[3]) || NEAR(dir[3], dir[0]))
//         return 'E';
//     if (areAnglesOpposite(dir[0], dir[1]) || areAnglesOpposite(dir[1], dir[2]) || areAnglesOpposite(dir[2], dir[3]) || areAnglesOpposite(dir[3], dir[0]))
//         return 'E';

    if (arePointsCollinear(s[0], s[1], s[2], s[3]))
        return 'E';

    if(areEdgesCrossing(s[0], s[1], s[2], s[3]))
        return 'M';

    auto p1 = s[0];
    auto p2 = s[1];
    auto p3 = s[2];
    auto p4 = s[3];

    long long d1 = distanceSquared(p1, p2);
    long long d2 = distanceSquared(p2, p3);
    long long d3 = distanceSquared(p3, p4);
    long long d4 = distanceSquared(p4, p1);
    long long diagonal1 = distanceSquared(p1, p3);
    long long diagonal2 = distanceSquared(p2, p4);

    if (d1 == d2 && d2 == d3 && d3 == d4)
    {
        if (areVectorsPerpendicular(vec(p1, p2), vec(p2, p3)))
            return 'N';            
        return 'R';
    }
    if (d1 == d3 && d2 == d4)
    {
        // parallelogramma
        if (areVectorsPerpendicular(vec(p1, p2), vec(p2, p3)))
            return 'T';            
        return 'P';
    }
    if (areVectorsPerpendicular(vec(p1, p3), vec(p2, p4)))
    {
        // deltoid
        return 'D';
    }
    if( areVectorsParallel(vec(p1, p2), vec(p3, p4)) || 
        areVectorsParallel(vec(p2, p3), vec(p4, p1)))
        return 'Z';

//     if (arePointsFormingSquare(s[0], s[1], s[2], s[3]))
//         return 'N';

//     if (arePointsFormingRhomboid(s[0], s[1], s[2], s[3]))
//         return 'R';

//     if (arePointsFormingRectangle(s[0], s[1], s[2], s[3]))
//         return 'T';

    return 'L';
}

void Solve(FILE* f)
{
    vector<pair<ll, ll>> s;
    for (;;)
    {
        s.clear();
        ll x, y;
        if (fscanf(f, "%lld %lld", &x, &y) <= 0)
            break;
        s.emplace_back(x, y);
        FOR(i, 3)
        {
            fscanf(f, "%lld %lld", &x, &y);
            s.emplace_back(x, y);
        }
        printf("%c", check(s));
    }
    printf("\n");
}


int main()
{
//     FILE* f = fopen("negyszog/test.txt", "r");
//     Solve(f);
//     printf("NNNDLTEZDTRTLMPPTLMMZNTLPMNLMMPMTEELTLMRNRLEZELNMD\n");
// 
//     return 0;


    vector<string> tasks;
    unordered_map<string, string> solutions;
    for (const auto& entry : fs::directory_iterator("negyszog"))  // <<<<--------------------------------------- folder name
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




