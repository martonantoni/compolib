#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* main_delimeters = " ,:";
bool main_allow_empty_fields = false;

struct cEdge
{
	int from, to;
	ll capacity = 1, used = 0;

	cEdge(int from, int to, ll capacity) : from(from), to(to), capacity(capacity), used(0) {}
	cEdge() {}
};

struct cNode
{
	vector<pair<int, bool>> edges; // edge index, is_reversed
};

vector<cEdge> edges;
vector<cNode> nodes;


namespace MaxFlow
{
	void AddEdge(int from, int to, ll capacity)
	{
		int edge_index = (int)edges.size();
		edges.emplace_back(from, to, capacity);
		nodes[from].edges.emplace_back(edge_index, false);
		nodes[to].edges.emplace_back(edge_index, true);
	}

	struct cBFSItem
	{
		size_t from_index; // in bfs data
		pair<int, bool> edge;
		ll capacity;
		cBFSItem() {}
		cBFSItem(size_t from_index, pair<int, bool> edge, ll capacity) : from_index(from_index), edge(edge), capacity(capacity) {}
	};
	vector<cBFSItem> bfs_data;
	vector<int> visit_timestamps;
	int visit_timestamp = 0;

	void ImproveEdge(const pair<int, bool>& edge, ll delta_capacity)
	{
		if (edge.second)
			edges[edge.first].used -= delta_capacity;
		else
			edges[edge.first].used += delta_capacity;
	}

	bool ImproveStep(int source, int sink)
	{
		++visit_timestamp;
		visit_timestamps[source] = visit_timestamp;
		size_t read_index = 0;
		bfs_data.clear();
		bfs_data.push_back(cBFSItem(numeric_limits<size_t>::max(), pair<int, bool>(-1, false), numeric_limits<int>::max()));
		for (;;)
		{
			size_t end_read_index = bfs_data.size();
			while (read_index != end_read_index)
			{
				cBFSItem& bfs_item = bfs_data[read_index];
				int edge_index = bfs_item.edge.first;
				int node_index = edge_index == -1 ? source : (bfs_item.edge.second ? edges[edge_index].from : edges[edge_index].to);
				for (vector<pair<int, bool> >::const_iterator i = nodes[node_index].edges.begin(), iend = nodes[node_index].edges.end(); i != iend; ++i)
				{
					const cEdge& edge = edges[i->first];
					int target_node_index = i->second ? edge.from : edge.to;
					if (visit_timestamps[target_node_index] == visit_timestamp)
						continue; // already visited
					ll free_capacity = i->second ? edge.used : edge.capacity - edge.used;
					if (!free_capacity)
						continue;
					if (target_node_index == sink)
					{
						ll delta_capacity = min(free_capacity, bfs_item.capacity);
						ImproveEdge(*i, delta_capacity);
						for (size_t bi = read_index;; bi = bfs_data[bi].from_index)
						{
							if (bfs_data[bi].edge.first == -1)
								break;
							ImproveEdge(bfs_data[bi].edge, delta_capacity);
						}
						return true;
					}
					bfs_data.push_back(cBFSItem(read_index, *i, min(free_capacity, bfs_item.capacity)));
					visit_timestamps[target_node_index] = visit_timestamp;
				}
				++read_index;
			}
			if (bfs_data.size() == end_read_index)
				return false; // cannot improve further
		}
	}

	ll GetFlow(int source) // call Solve first!
	{
		ll flow = 0;
		for (auto ei : nodes[source].edges)
		{
			flow += edges[ei.first].used;
		}
		return flow;
	}

	void Solve(int source, int sink)
	{
		bfs_data.clear();
		visit_timestamps.clear();
		visit_timestamp = 0;
		for (auto& edge : edges)
			edge.used = 0;

		bfs_data.reserve(nodes.size());
		visit_timestamps.resize(nodes.size(), -1);
		while (ImproveStep(source, sink));
	}

}


void solveFirst()
{
	fflush(out);

    ll res = 0;

	nodes.clear();
	edges.clear();

	unordered_map<string, ll> names;
	auto nameIdx = [&](const string& name)
		{
			if (names.contains(name))
				return names[name];
			nodes.emplace_back();
			ll new_index = names.size();
			names[name] = new_index;

			return new_index;
		};

	auto addEdge = [&](const string& a, const string& b, int capacity = 1) -> ll
		{
			ll ai = nameIdx(a);
			ll bi = nameIdx(b);
			MaxFlow::AddEdge(ai, bi, capacity);
			MaxFlow::AddEdge(bi, ai, capacity);
			return ai;
		};

    for (auto& l : ls)
    {
		for (const auto& name : l.s | views::drop(1))
		{
			addEdge(l.s[0], name);
		}
    }
	

	for (int source = 0; source < nodes.size(); ++source)
	{
		printf("%d\n", source);
		for (int sink = source + 1; sink < nodes.size(); ++sink)
		{
			MaxFlow::Solve(source, sink);
			int flow = MaxFlow::GetFlow(source);
			if (flow == 3)
			{
			//	P("**********************");
				P("between %d and %d: %d", source, sink, flow); fflush(out);


				auto tryRemove = [&](int ei)
					{
						edges[ei].capacity = 0;
						MaxFlow::Solve(source, sink);
						int new_flow = MaxFlow::GetFlow(source);
						if (new_flow < flow)
						{
							flow = new_flow;
							P("found edge to remove. flow: %d", flow);
							return true;
						}
						edges[ei].capacity = 1;
						return false;
					};


				for (int ei = 0; ei < edges.size() && flow > 0; ++ei)
				{
					tryRemove(ei);
				}


				ll first_group_size = 1;
				for (int tested = 1; tested < nodes.size(); ++tested)
				{
					MaxFlow::Solve(source, tested);
					if (MaxFlow::GetFlow(tested) >= 1)
						++first_group_size;
				}
				ll second_group_size = nodes.size() - first_group_size;
				P("groups: %lld, %lld, result: %lld", first_group_size, second_group_size, first_group_size * second_group_size);
				return;
			}
		}
	}


//    P("result: %lld", res);
}



void solveSecond()
{
    ll res = 0;
    P("result: %lld", res);
}


 void solve(bool first) { first ? solveFirst() : solveSecond(); }

