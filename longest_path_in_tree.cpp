struct cNode
{
    vector<int> to;
};
vector<cNode> nodes;


pair<int, int> GetDepth(int from, int i) // endpoint, length
{
    cNode& node = nodes[i];
    pair<int, int> best(-1, -1);
    for (auto to : node.to)
    {
        if (to != from)
        {
            auto d = GetDepth(i, to);
            if (d.second > best.second)
                best = d;
        }
    }
    if (best.first == -1)
    {
        return { i, 0 };
    }
    return { best.first, best.second + 1 };
}

pair<int, int> GetLongestPathEndpoints()
{
    auto left = GetDepth(-1, 0);
    auto right = GetDepth(-1, left.first);
// length would be:  >> right.second <<
    return { left.first, right.first };
}