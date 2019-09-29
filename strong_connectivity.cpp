struct cNode
{
	vector<int> to;
};

vector<cNode> nodes;

namespace Kosaraju  // NOT COMPO TESTED
{
	struct cComponent
	{
		vector<int> nodes;

		cComponent(vector<int> &&nodes): nodes(std::move(nodes)) {}
	};

	struct cNodeData
	{
		int process_state=0;
		vector<int> from;
	};
	vector<cNodeData> augmentation;
	vector<int> processed_nodes;
	vector<int> current_component;

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
		for(int i=0;i<number_of_nodes;++i)
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
		for(int i=number_of_nodes-1; i>=0; --i)
		{
			int node_index=processed_nodes[i];
			if(augmentation[node_index].process_state==1)
			{
				Step2_DepthFirstSearch(node_index);
				components.emplace_back(std::move(current_component));
			}
		}

		return components;
	}
}

void Solve()
{
	RII(n,m);
	nodes.resize(n);
	for(int i=0; i<m; ++i)
	{
		RII(u, v);
		--u; --v;
		nodes[u].to.emplace_back(v);
	}
	auto components=Kosaraju::Run();
	for(auto &component: components)
	{
		printf("component: ");
		for(auto node_index: component.nodes)
			printf("%d, ", node_index+1);
		printf("\n");
	}
}

/*
test data:
7 10
1 2
2 1
1 4
2 5
5 4
6 5
3 2
6 3
3 7
7 6
*/