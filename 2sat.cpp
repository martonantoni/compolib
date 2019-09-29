struct cNode
{
	vector<int> to;
};

vector<cNode> nodes;

namespace Kosaraju
{
	struct cComponent
	{
		vector<int> nodes;

		cComponent(vector<int> &&nodes): nodes(std::move(nodes)) {}
	};

	struct cNodeData
	{
		int process_state=0;
		int component_index=0;
		vector<int> from;
	};
	vector<cNodeData> augmentation;
	vector<int> processed_nodes;
	vector<int> current_component;
	int current_component_index=0;

	void Step1_DepthFirstSearch(int node_index)
	{
		augmentation[node_index].process_state=1;
		for(auto to: nodes[node_index].to)
		{
			if(augmentation[to].process_state==0)
				Step1_DepthFirstSearch(to);
		}
		processed_nodes.emplace_back(node_index);
	}

	void Step2_DepthFirstSearch(int node_index)
	{
		current_component.emplace_back(node_index);
		augmentation[node_index].process_state=2;
		augmentation[node_index].component_index=current_component_index;
		for(auto to: augmentation[node_index].from)
		{
			if(augmentation[to].process_state==1)
				Step2_DepthFirstSearch(to);
		}
	}

	vector<cComponent> Run()
	{
		int number_of_nodes=(int)nodes.size();
		augmentation.clear();
		augmentation.resize(number_of_nodes);
		// create reversed graph:
		for(int i=0; i<number_of_nodes; ++i)
		{
			for(auto to: nodes[i].to)
			{
				augmentation[to].from.emplace_back(i);
			}
		}
		// Search 1, create list of processed nodes
		processed_nodes.clear();
		processed_nodes.reserve(number_of_nodes);
		for(int i=0; i<number_of_nodes; ++i)
		{
			if(augmentation[i].process_state==1)
				continue;
			Step1_DepthFirstSearch(i);
		}
		// Search 2, create connected components
		vector<cComponent> components;
		current_component_index=0;
		for(int i=number_of_nodes-1; i>=0; --i)
		{
			int node_index=processed_nodes[i];
			if(augmentation[node_index].process_state==1)
			{
				Step2_DepthFirstSearch(node_index);
				components.emplace_back(std::move(current_component));
				++current_component_index;
			}
		}

		return components;
	}
}

namespace TwoSat
{

	void Reset()
	{
		nodes.clear();
	}

	void AddClause(int a_x, bool a_negated, int b_x, bool b_negated)
	{
		int x_lower_bound=max(a_x, b_x)*2+1;
		if(x_lower_bound>=nodes.size())
			nodes.resize(x_lower_bound+1);
		int node_index_a=a_x*2+(a_negated?1:0);
		int node_index_not_a=a_x*2+(a_negated?0:1);
		int node_index_b=b_x*2+(b_negated?1:0);
		int node_index_not_b=b_x*2+(b_negated?0:1);
		nodes[node_index_not_a].to.emplace_back(node_index_b);
		nodes[node_index_not_b].to.emplace_back(node_index_a);
	}

	vector<int> Solve()
	{
		auto components=Kosaraju::Run(); // generates the components in topological sort order
		for(size_t i=0, iend=nodes.size(); i!=iend; i+=2)
		{
			if(Kosaraju::augmentation[i].component_index==Kosaraju::augmentation[i+1].component_index)
				return{}; // unsolvable
		}
		vector<int> result(nodes.size()/2, -1);
		for(int i=(int)components.size()-1; i>=0; --i) // go through components in reverse topological sort order
		{
			for(auto node_index: components[i].nodes)
			{
				int x_index=node_index/2;
				if(result[x_index]==-1)
				{
					if(node_index&1)
					{
						result[x_index]=0;
					}
					else
					{
						result[x_index]=1;
					}
				}
			}
		}
		return result;
	}

	void Test()
	{
		TwoSat::AddClause(2, false, 1, true);
		TwoSat::AddClause(1, true, 2, true);
		TwoSat::AddClause(1, false, 3, false);
		TwoSat::AddClause(2, true, 3, true);
		TwoSat::AddClause(1, false, 4, false);

		auto result=TwoSat::Solve();
	}
}
