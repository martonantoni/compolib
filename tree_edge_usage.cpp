// how many times an edge is used in ALL simple path within the tree

struct cNode
{
    vector<int> to;
    vector<int> nodes_under_edge;
};

vector<cNode> nodes;
vector<ll> edge_usage;
ll number_of_nodes;

ll Calc(int ni, int from)
{
    cNode& node = nodes[ni];
    ll sum = 1;
    for (int to : node.to)
    {
        if(to==from)
            continue;
        ll under_edge = Calc(to, ni);
        edge_usage.emplace_back(under_edge * (number_of_nodes - under_edge));
        sum += under_edge;
    }
    return sum;
}

void Solve()
{
    scanf("%lld", &number_of_nodes);
    nodes.clear();
    nodes.resize(number_of_nodes);
    edge_usage.clear();
    edge_usage.reserve(number_of_nodes - 1);
    FOR(i, number_of_nodes - 1)
    {
        RII(u, v);
        --u; --v;
        nodes[u].to.emplace_back(v);
        nodes[v].to.emplace_back(u);
    }
    Calc(0, -1);