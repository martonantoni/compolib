struct cGraphEdge
{
	ll to;
	ll weight;
	cGraphEdge(ll t, ll w): to(t), weight(w) {}
};

struct cNode
{
	bool is_connected=false;
	vector<cGraphEdge> edges;
	vector<cGraphEdge> tree_edges;
};

vector<cNode> graph_nodes;

vector<pair<cGraphEdge,ll /*from*/>> frontier;





	cin >> n >> m;
	graph_nodes.resize(n);
	for(ll i=0; i<m; ++i)
	{
		ll from, to, w;
		cin >> from >> to >> w;
		--from; --to;
		graph_nodes[from].edges.emplace_back(to, w);
		graph_nodes[to].edges.emplace_back(from, w);
	}
	int connected_nodes=1;
	frontier.reserve(graph_nodes[0].edges.size());
	for(auto &e: graph_nodes[0].edges)
		frontier.emplace_back(e, 0);
	sort(frontier.begin(), frontier.end(), [](auto &a, auto &b) { return a.first.weight>b.first.weight; });
	graph_nodes[0].is_connected=true;
	while(connected_nodes<n)
	{
		// pop redundant edges:
		while(graph_nodes[frontier.back().first.to].is_connected)
			frontier.pop_back();

		ll new_node=frontier.back().first.to;
		ll from=frontier.back().second;
		graph_nodes[from].tree_edges.emplace_back(frontier.back().first);
		graph_nodes[new_node].is_connected=true;
		// add new edges to frontier

// makes it slower:		frontier.erase(remove_if(frontier.begin(), frontier.end(), [new_node](auto &e) { return e.first.to==new_node; }), frontier.end());

		for(auto &e: graph_nodes[new_node].edges)
		{
			if(!graph_nodes[e.to].is_connected)
			{
				auto i=lower_bound(frontier.begin(), frontier.end(), e.weight, [](auto &e, int w) { return e.first.weight>w; });
//makes it slower:				if(none_of(i, frontier.end(), [to=e.to](auto &e) { return e.first.to==to; }))
				frontier.emplace(i, e, new_node);
			}
		}
		++connected_nodes;
	}

