
struct cEdge
{
	int from, to;
	ll capacity, used;

	cEdge(int from, int to, ll capacity): from(from), to(to), capacity(capacity), used(0) {}
	cEdge() {}
};

struct cNode
{
	vector<pair<int,bool> > edges; // edge index, is_reversed
};

vector<cEdge> edges;
vector<cNode> nodes;

void AddEdge(int from, int to, ll capacity)
{
    int edge_index = (int)edges.size();
    edges.emplace_back(from, to, capacity);
    nodes[from].edges.emplace_back(edge_index, false);
    nodes[to].edges.emplace_back(edge_index, true);
}

namespace MaxFlow
{

	struct cBFSItem
	{
		size_t from_index; // in bfs data
		pair<int,bool> edge;
		ll capacity;
		cBFSItem() {}
		cBFSItem(size_t from_index, pair<int,bool> edge, ll capacity): from_index(from_index), edge(edge), capacity(capacity) {}
	};
	vector<cBFSItem> bfs_data;
	vector<int> visit_timestamps;
	int visit_timestamp=0;

	void ImproveEdge(const pair<int, bool> &edge, ll delta_capacity)
	{
		if(edge.second)
			edges[edge.first].used-=delta_capacity;
		else
			edges[edge.first].used+=delta_capacity;
	}

	bool ImproveStep(int source, int sink)
	{
		++visit_timestamp;
		visit_timestamps[source]=visit_timestamp;
		size_t read_index=0;
		bfs_data.clear();
		bfs_data.push_back(cBFSItem(numeric_limits<size_t>::max(), pair<int, bool>(-1, false), numeric_limits<int>::max()));
		for(;;)
		{
			size_t end_read_index=bfs_data.size();
			while(read_index!=end_read_index)
			{
				cBFSItem &bfs_item=bfs_data[read_index];
				int edge_index=bfs_item.edge.first;
				int node_index=edge_index==-1?source:(bfs_item.edge.second?edges[edge_index].from:edges[edge_index].to);
				for(vector<pair<int, bool> >::const_iterator i=nodes[node_index].edges.begin(), iend=nodes[node_index].edges.end(); i!=iend; ++i)
				{
					const cEdge &edge=edges[i->first];
					int target_node_index=i->second?edge.from:edge.to;
					if(visit_timestamps[target_node_index]==visit_timestamp)
						continue; // already visited
					ll free_capacity=i->second?edge.used:edge.capacity-edge.used;
					if(!free_capacity)
						continue;
					if(target_node_index==sink)
					{
						ll delta_capacity=min(free_capacity, bfs_item.capacity);
						ImproveEdge(*i, delta_capacity);
						for(size_t bi=read_index;;bi=bfs_data[bi].from_index)
						{
							if(bfs_data[bi].edge.first==-1)
								break;
							ImproveEdge(bfs_data[bi].edge, delta_capacity);
						}
						return true;
					}
					bfs_data.push_back(cBFSItem(read_index, *i, min(free_capacity, bfs_item.capacity)));
					visit_timestamps[target_node_index]=visit_timestamp;
				}
				++read_index;
			}
			if(bfs_data.size()==end_read_index)
				return false; // cannot improve further
		}
	}

	ll GetFlow(int source) // call Solve first!
	{
		ll flow = 0;
		for(auto ei : nodes[source].edges)
		{
			flow += edges[ei.first].used;
		}
		return flow;
	}

	void Solve(int source, int sink)
	{
		bfs_data.reserve(nodes.size());
		visit_timestamps.resize(nodes.size(), -1);
		while(ImproveStep(source, sink));
	}
    
}

void Solve()
{
	int n, m;
	while(scanf("%d %d", &m, &n)==2)
	{
		edges.clear();
		edges.reserve(m);
		nodes.clear();
		nodes.resize(n);
		FOR(i, m)
		{
			RIII(s, e, c);
			--s; --e;
			edges.push_back(cEdge(s, e, c));
			nodes[s].edges.push_back(pair<int,bool>(i,false));
			nodes[e].edges.push_back(pair<int,bool>(i,true));
		}
		MaxFlow::Solve(0, n-1);
		ll flow=0;
		for(vector<pair<int, bool> >::const_iterator i=nodes[0].edges.begin(), iend=nodes[0].edges.end(); i!=iend; ++i)
		{
			if(!i->second)
				flow+=edges[i->first].used;
		}
		printf("%lld\n", flow);
	}
}

int Init()
{
	//	RI(t);
	int t=1;
	return t;
}

int main()
{
	//	std::ios::sync_with_stdio(false);
	int t=Init();
	for(int tc=1; tc<=t; ++tc)
	{
		//		printf("Case #%d: ", tc);
		Solve();
	}
}