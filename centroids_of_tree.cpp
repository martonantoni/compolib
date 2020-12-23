struct cNode
{
    vector<int> to;
};
vector<cNode> nodes;

vector<int> centroids; // either 1 or 2
int FindCentroids_DFS(int ni, int from)
{
    int sub_size = 1; // counting this node too
    bool is_centroid = true;
    auto& node = nodes[ni];
    for (int to : node.to)
    {
        if (to == from)
            continue;
        int to_sub_size = FindCentroids_DFS(to, ni);
        if (to_sub_size > nodes.size() / 2)
            is_centroid = false;
        sub_size += to_sub_size;
    }
    int size_toward_from = nodes.size() - sub_size;
    if (is_centroid && size_toward_from <= nodes.size() / 2)
    {
        centroids.emplace_back(ni);
    }
    return sub_size;
}

void FindCentroids()
{
    centroids.clear();
    FindCentroids_DFS(0, -1);
}

void Solve()
{
    RI(n);
    nodes.clear();
    nodes.resize(n);
    FOR(i, n - 1)
    {
        RII(u, v);
        --u; --v;
        nodes[u].to.emplace_back(v);
        nodes[v].to.emplace_back(u);
    }
    FindCentroids();
