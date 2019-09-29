///                     HeavyLightDecomposition

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

struct cChain
{
	int root;
	int length=0;

	vector<int> nodes;

	cChain(int root): root(root) {}
	///-----------
};

vector<cChain> chains;

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



void HeavyLightDecomposition(int node_index, int chain_index)
{
	auto &node=tree[node_index];
	auto &chain=chains[chain_index];
	++chain.length;
	node.chain=chain_index;
	chains[chain_index].nodes.emplace_back(node_index);
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

void HLD(int root=0)
{
/*  add if need to run HLD multiple times:
	for(auto& node: tree)
	{
		node.longest_subtree=-1;
		node.depth=0;
		node.top_height=0;
	}
	chains.clear();
*/
	chains.reserve(100'000);
	chains.emplace_back(root);
	
	CalculateTopHeights(0);
	HeavyLightDecomposition(root, 0);
}

struct cTraverseData
{
	int chain;
	int from, to; // index in chain
	cTraverseData(int chain, int from, int to): chain(chain), from(from), to(to) {}
	cTraverseData() {}
};
vector<cTraverseData> traverse_data_forward, traverse_data_backward;

void HLDTraverse(int u /*from*/, int v /*to*/) // fills out traverse_data_forward & traverse_data_backward
{
	traverse_data_forward.clear();
	traverse_data_backward.clear();
	for(;;)
	{
		int chain_u_index=tree[u].chain, chain_v_index=tree[v].chain;
		auto &chain_u=chains[chain_u_index], &chain_v=chains[chain_v_index];
		int index_in_chain_u=tree[chain_u.root].top_height-tree[u].top_height;
		int index_in_chain_v=tree[chain_v.root].top_height-tree[v].top_height;
		if(tree[u].chain==tree[v].chain)
		{
			if(index_in_chain_u>index_in_chain_v)
				traverse_data_forward.emplace_back(chain_u_index, index_in_chain_u, index_in_chain_v);
			else
				traverse_data_backward.emplace_back(chain_u_index, index_in_chain_u, index_in_chain_v);
			break;
		}
		if(tree[chain_u.root].depth>tree[chain_v.root].depth)
		{
			traverse_data_forward.emplace_back(chain_u_index, index_in_chain_u, 0);
			u=tree[chain_u.root].parent;
		}
		else
		{
			traverse_data_backward.emplace_back(chain_v_index, 0, index_in_chain_v);
			v=tree[chain_v.root].parent;
		}
	}
}
