

#include "aoc.h"
#include "bmp.h"
#include "utils.h"

#include <bitset>

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

//template<typename cLocation, auto locationToIdx, auto nextLocations, size_t numberOfLocations>
//struct cDijkstra
//{
//    std::array<std::pair<int, int>, numberOfLocations> mCosts; // best_cost, from
//
//    void run(const cLocation& from)
//    {
//        struct cState
//        {
//            int cost;
//            cLocation loc;
//            cLocation from;
//        };
//        mCosts.fill({ std::numeric_limits<int>::max(), -1 });
//        mCosts[locationToIdx(from)] = { 0, -1 };
//        std::vector<cState> states;
//        states.push_back({ 0, from, {} });
//        while (!states.empty())
//        {
//            auto [cost, loc, from] = states.front();
//            states.pop_heap();
//            std::ranges::pop_heap(states, std::greater<>(), &cState::cost);
//            if (cost > mCosts[locationToIdx(loc)].first)
//                continue;
//            mCosts[locationToIdx(loc)] = { cost, locationToIdx(from) };
//            for (auto&& [nextLoc, moveCost] : nextLocations(loc))
//            {
//                auto nextIdx = locationToIdx(next);
//                auto nextCost = cost + moveCost;
//                if (nextCost < mCosts[nextIdx].first)
//                {
//                    std::ranges::push_heap(states, std::greater<>(), &cState::cost);
//                }
//            }
//        }
//    }
//};

template<typename cLocation>
auto runDijkstra(const cLocation& from, auto locationToIdx, auto nextLocations, size_t numberOfLocations)
{
    struct cNode
    {
        ll cost = std::numeric_limits<ll>::max();
        cLocation loc;
        cLocation from;
        bool done = false;
    };
    int fromIdx = locationToIdx(from);
    std::vector<cNode> nodes(numberOfLocations);
    nodes[fromIdx] = { 0, from, from };
    std::vector<int> frontier;
    frontier.emplace_back(fromIdx);

    while (!frontier.empty())
    {
        auto checkedNodeIdx = frontier.front();
        std::ranges::pop_heap(frontier, [&](int a, int b) { return nodes[a].cost > nodes[b].cost; });
        frontier.pop_back();
        auto& checkedNode = nodes[checkedNodeIdx];
        if (checkedNode.done)
            continue;
        checkedNode.done = true;
        for (auto&& [nextLoc, moveCost] : nextLocations(checkedNode.loc))
        {
            auto nextIdx = locationToIdx(nextLoc);
            auto nextCost = checkedNode.cost + moveCost;
            if (nextCost < nodes[nextIdx].cost)
            {
                nodes[nextIdx].cost = nextCost;
                nodes[nextIdx].from = checkedNode.loc;
                nodes[nextIdx].loc = nextLoc;
                frontier.emplace_back(nextIdx);
                std::ranges::push_heap(frontier, [&](int a, int b) { return nodes[a].cost > nodes[b].cost; });
            }
        }
    }
    return nodes;
}


int findBest(cImage<char>& img, cPosition end_pos)
{
    //cImage<int> best(img, (int)std::numeric_limits<int>::max());
    //vector<pair<cPosition, int>> f;
    //f.emplace_back(cPosition{ 0,0 }, 0);
    //while (!f.empty())
    //{
    //    auto [pos, cost] = f.front();
    //    pop_heap(ALL(f), [](auto& l, auto& r) { return l.second > r.second; });
    //    f.pop_back();
    //    if (pos == end_pos)
    //    {
    //        return cost;
    //    }
    //    for (auto np : img.neighbours(pos))
    //    {
    //        if (img[np] == '.')
    //        {
    //            if (best[np] <= cost + 1)
    //                continue;
    //            best[np] = cost + 1;
    //            f.emplace_back(np, cost + 1);
    //            push_heap(ALL(f), [](auto& l, auto& r) { return l.second > r.second; });
    //        }
    //    }
    //}
    //return -1;

    auto costs = runDijkstra<cPosition>(
        cPosition{ 0, 0 },
        [w = img.w](const cPosition p) { return static_cast<int>(p.col + p.row * w); },
        [&img](const cPosition p) 
        {
            return std::views::all(neighbour4Positions) |
                std::views::transform([p](cPosition d) { return p + d; }) |
                std::views::filter([&img](cPosition np) { return img.isValidPos(np) && img[np] == '.'; }) |
                std::views::transform([](cPosition np) { return std::pair{ np, 1 }; });
        },
        img.w * img.h);

    return costs[img.w - 1 + (img.h - 1) * img.w].cost;
}

void solve(bool first)
{
    ll res = 0;

    int w = is_example ? 7 : 71;
    int h = w;

    vector<cPosition> falls;
    cPosition start_pos(0, 0);
    cPosition end_pos(w - 1, h - 1);

    for (auto& l : ls)
    {
        auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;
        falls.emplace_back(l.i[0], l.i[1]);
    }

    cImage<char> img;
    img.w = w;
    img.h = h;
    img.cells.resize(w * h, '.');


    int to = is_example ? 12 : 1024;

    for (int i = 0; i < to; ++i)
    {
        img[falls[i]] = '#';
    }
    if (first)
    {
        RESULT(findBest(img, end_pos));
    }
    else
    {
        auto res = value_lower_bound(to, falls.size(),
            [&](int to)
            {
                for (int i = 0; i < falls.size(); ++i)
                    img[falls[i]] = i < to ? '#' : '.';
                return findBest(img, end_pos) == -1 ? lower : higher;
            });

        P("res: {},{}", falls[res].row, falls[res].col);
    }
}