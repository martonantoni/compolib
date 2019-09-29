
// https://www.hackerrank.com/challenges/heavy-light-white-falcon

struct cNode
{
	vector<int> to;
// stuff needed for HLD:
	int depth=0;
	int top_height=0;
	int longest_subtree=-1;
	int parent; // not needed, but a trivial to calculate byproduct
	int chain;
///-----------
};

vector<cNode> tree;

void CalculateTopHeights(int root)
{
	vector<pair<int, int>> stack; // node, from 
	stack.reserve(tree.size());
	stack.emplace_back(root, -1);
	while(!stack.empty())
	{
		auto top=stack.back(); 
		auto &node=tree[top.first];
		if(node.top_height!=0||(node.to.size()==1&&top.second!=-1))
		{
			if(top.first!=root)
			{
				auto &parent_node=tree[top.second];
				if(parent_node.top_height<node.top_height+1)
				{
					parent_node.top_height=node.top_height+1;
					parent_node.longest_subtree=top.first;
				}
			}
			node.parent=top.second;
			stack.pop_back();
		}
		else
		{
			for(auto &to: tree[top.first].to)
			{
				if(to!=top.second)
				{
					tree[to].depth=node.depth+1;
					stack.emplace_back(to, top.first);
				}
			}
		}
	}
}

struct cChain
{
	int root;
	int length=0;

	tSegmentTree<cSegmentData, cSegmentTreeBase> segment_tree;

	cChain(int root): root(root) {}
};

vector<cChain> chains;

void HeavyLightDecomposition(int node_index, int chain_index)
{
	auto &node=tree[node_index];
	auto &chain=chains[chain_index];
	++chain.length;
	node.chain=chain_index;
	if(node.longest_subtree==-1)
		return;
	HeavyLightDecomposition(node.longest_subtree, chain_index);
	for(auto i: node.to)
	{
		if(i!=node.parent&&i!=node.longest_subtree)
		{
			chains.emplace_back(i);
			HeavyLightDecomposition(i, ((int)chains.size())-1);
		}
	}
}

void HeavyLightDecomposition(int root)
{
	chains.reserve(5000);
	chains.emplace_back(root);
	HeavyLightDecomposition(root, 0);
	cLogPerformance_Guard Guard("init segtrees");
	for(auto &chain: chains)
	{
		chain.segment_tree.Init(chain.length, [](auto Items, int Size) {});
	}
}

template<class F>
void ForSegmentsBetweenNodes(int u, int v, F Func)
{
	for(;;)
	{
		auto &chain_u=chains[tree[u].chain], &chain_v=chains[tree[v].chain];
		int index_in_chain_u=tree[chain_u.root].top_height-tree[u].top_height;
		int index_in_chain_v=tree[chain_v.root].top_height-tree[v].top_height;
		if(tree[u].chain==tree[v].chain)
		{
			if(index_in_chain_u>index_in_chain_v)
				swap(index_in_chain_u, index_in_chain_v);
			chain_u.segment_tree.ForInterval(index_in_chain_u, index_in_chain_v, Func);
			return;
		}
		if(tree[chain_u.root].depth>tree[chain_v.root].depth)
		{
			chain_u.segment_tree.ForInterval(0, index_in_chain_u, Func);
			u=tree[chain_u.root].parent;
		}
		else
		{
			chain_v.segment_tree.ForInterval(0, index_in_chain_v, Func);
			v=tree[chain_v.root].parent;
		}
	}
}

void ReadTree(int n)
{
	tree.resize(n);
	for(int i=1; i<n; ++i)
	{
		RII(a, b);
//		--a; --b;
		tree[a].to.emplace_back(b);
		tree[b].to.emplace_back(a);
	}
	{
		cLogPerformance_Guard Guard("calculate top heights");
		CalculateTopHeights(0);
	}
	{
		cLogPerformance_Guard Guard("HLD");
		HeavyLightDecomposition(0);
	}
}

void Solve()
{
	RII(n, q);
	ReadTree(n);
	cLogPerformance_Guard Guard("Queries");
	int line=0;
	for(int i=0; i<q; ++i)
	{
		RI(type);
		if(type==1)
		{
			RII(u, x);
//			--u;
			auto &node=tree[u];
			auto &chain=chains[node.chain];
			int index_in_chain=tree[chain.root].top_height-node.top_height;
			chain.segment_tree.Update(index_in_chain, [x](auto &item) { item.x=x; });
		}
		else
		{
			RII(u, v);
//			--u; --v;
			int max_value=numeric_limits<int>::min();
			ForSegmentsBetweenNodes(u, v, [&max_value](auto &item)
			{
				max_value=max(item.x, max_value);
			});
			printf("%d\n", max_value);
		}
	}
}
