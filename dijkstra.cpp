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
    using cIndexType = decltype(locationToIdx(from));
    std::vector<std::pair<ll, cIndexType>> frontier; // cost, index
    if constexpr (requires { from.begin(); from.end(); })
    {
        for (auto&& loc : from)
        {
            auto idx = locationToIdx(loc);
            nodes[idx] = { 0, loc, loc };
            frontier.emplace_back(0, idx);
        }
        std::ranges::make_heap(frontier, std::greater{});
    }
    else
    {
        auto fromIdx = locationToIdx(from);
        nodes[fromIdx] = { 0, from, from };
        frontier.emplace_back(0, fromIdx);
    }

    while (!frontier.empty())
    {
        auto [cost, checkedNodeIdx] = frontier.front();
        std::ranges::pop_heap(frontier, std::greater{});
        frontier.pop_back();
        auto& checkedNode = nodes[checkedNodeIdx];
        if (checkedNode.done)
            continue;
        checkedNode.done = true;
        for (auto&& [nextLoc, moveCost] : nextLocations(checkedNode.loc))
        {
            auto nextIdx = locationToIdx(nextLoc);
            auto nextCost = cost + moveCost;
            if (nextCost < nodes[nextIdx].cost)
            {
                nodes[nextIdx].cost = nextCost;
                nodes[nextIdx].from = checkedNode.loc;
                nodes[nextIdx].loc = nextLoc;
                frontier.emplace_back(nextCost, nextIdx);
                std::ranges::push_heap(frontier, std::greater{});
            }
        }
    }
    return nodes;
}

std::string reconstructPath(const auto& nodes, auto locationToIdx, auto fromIdx, auto toIdx, auto stepToString)
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







        ofstream out("dijkstra_result.txt");
        for(int row = 0; row <archipelago.h;++row)
        {
            for (int col = 0; col < archipelago.w; ++col)
            {
                print(out, "+--------");
            }
            print(out, "\n");
            for (int col = 0; col < archipelago.w; ++col)
            {
                print(out, "|        ");
            }
            print(out, "\n");




            for(int col=0;col<archipelago.w;++col)
            {
                cPosition p{ row, col };
                auto idx = posToIdx(p);
                if(res[idx].cost == std::numeric_limits<ll>::max())
                {
                    print(out, "|    X   ");
                }
                else
                {
                    print(out, "| {:6} ", res[idx].cost >> shift);
                }
            }
            print(out, "\n");
            for (int col = 0; col < archipelago.w; ++col)
            {
                cPosition p{ row, col };
                auto idx = posToIdx(p);
                if (res[idx].cost == std::numeric_limits<ll>::max())
                {
                    print(out, "|        ");
                }
                else
                {
                    cPosition from = res[idx].from;
                    cPosition diff = p - from;
                    char c = '?';
                    if (diff == direction_N) c = 'S';
                    else if (diff == direction_S) c = 'N';
                    else if (diff == direction_E) c = 'W';
                    else if (diff == direction_W) c = 'E';
                    print(out, "|   {}    ", c);
                }
            }
            print(out, "\n");
