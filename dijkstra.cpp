template<typename cLocation>
auto runDijkstra(const auto& from, auto locationToIdx, auto nextLocations, size_t numberOfLocations)
{
    struct cNode
    {
        ll cost = std::numeric_limits<ll>::max();
        cLocation loc;
        cLocation from;
        bool done = false;
    };
    std::vector<cNode> nodes(numberOfLocations);
    std::vector<int> frontier;
    if constexpr (requires { from.begin(); from.end(); })
    {
        for(auto&& loc : from)
        {
            auto idx = locationToIdx(loc);
            nodes[idx] = { 0, loc, loc };
            frontier.emplace_back(idx);
        }
        std::ranges::make_heap(frontier, [&](int a, int b) { return nodes[a].cost > nodes[b].cost; });
    }
    else
    {
        int fromIdx = locationToIdx(from);
        nodes[fromIdx] = { 0, from, from };
        frontier.emplace_back(fromIdx);
    }

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

std::string reconstructPath(const auto& nodes, auto locationToIdx, int fromIdx, int toIdx, auto stepToString)
{
    std::string moves;
    auto nodeIdx = toIdx;
    while (nodeIdx != fromIdx)
    {
        auto& node = nodes[nodeIdx];
        moves += stepToString(node.from, node.loc);
        nodeIdx = locationToIdx(node.from);
    }
    std::ranges::reverse(moves);
    return moves;
}


/////////////////////
// tested example:

int findBest(cImage<char>& img, cPosition end_pos)
{
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