// https://www.hackerearth.com/practice/algorithms/graphs/articulation-points-and-bridges/tutorial/

struct cNode
{
	vector<int> to;
	int discovery_time=-1;
	int lowest_reachable;
};

vector<cNode> nodes;
vector<int> articulation_points;
vector<pair<int, int>> bridges;

void FindArticulationPoints(int node_index, int from, int time)
{
	auto &node=nodes[node_index];
	node.discovery_time=time;
	node.lowest_reachable=time;
	bool is_articulation_point=false;
	int child_count=0;
	for(int to: node.to)
	{
		if(to!=from)
		{
			auto &to_node=nodes[to];
			if(to_node.discovery_time!=-1)
			{
				node.lowest_reachable=min(node.lowest_reachable, to_node.discovery_time);
			}
			else
			{
				++child_count;
				FindArticulationPoints(to, node_index, time+1);
				if(to_node.lowest_reachable>=time)
				{
					// no back edge from the subtree rooted at to => this node is articulation point
					is_articulation_point=true;
					// bridge:
					if(to_node.lowest_reachable>time)
						bridges.emplace_back(node_index, to);
				}
				node.lowest_reachable=min(node.lowest_reachable, nodes[to].lowest_reachable);
			}
		}
	}
	if(from==-1)
	{
		// root:
		if(child_count>=2)
		{
			articulation_points.emplace_back(node_index);
		}
	}
	else
	{
		if(is_articulation_point)
		{
			articulation_points.emplace_back(node_index);
		}
	}
}

void Solve()
{
	RII(n, m);
	nodes.resize(n);
	FOR(j, m)
	{
		RII(a, b);
		// zero based indexes!!!!
		nodes[a].to.emplace_back(b);
		nodes[b].to.emplace_back(a);
	}
	FindArticulationPoints(0, -1, 0);

	printf("%d\n", (int)articulation_points.size());
	sort(ALL(articulation_points));
	for(auto x: articulation_points)
		printf("%d ", x);
	for(auto b: bridges)
		if(b.first>b.second)
			swap(b.first, b.second);
	sort(ALL(bridges), less<>());
	printf("\n%d\n", (int)bridges.size());
	for(auto b: bridges)
		printf("%d %d\n", b.first, b.second);
}
