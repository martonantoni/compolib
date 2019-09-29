const int tab32[32] ={
	0,  9,  1, 10, 13, 21,  2, 29,
	11, 14, 16, 18, 22, 25,  3, 30,
	8, 12, 20, 28, 15, 17, 24,  7,
	19, 27, 23,  6, 26,  5,  4, 31 };

int log2_32(uint32_t value)
{
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	return tab32[(uint32_t)(value*0x07C4ACDD) >> 27];
}

template<class BASE>
class tRangeMinimumQuery : public BASE // Base must supply: int Select(int ai, int bi) const; function
{
	vector<vector<int>> min_indexes;
public:
	void Init(int Size)
	{
		auto log_size=log2_32(Size)+1;
		min_indexes.resize(log2_32(Size)+1);
		for(auto &v: min_indexes)
			v.resize(Size);
		iota(ALL(min_indexes[0]), 0);
		for(int j=1, rs2=1; j<log_size; ++j, rs2<<=1)
		{
			for(int i=0; i<Size; ++i)
			{
				if(i+rs2<Size)
				{
					min_indexes[j][i]=BASE::Select(min_indexes[j-1][i], min_indexes[j-1][i+rs2]);
				}
				else
					min_indexes[j][i]=min_indexes[j-1][i];
			}
		}
	}
	int GetRMQ(int left, int right)
	{
		if(right<left)
			swap(right, left);
		int range_size=right-left+1;
		int l32=log2_32(range_size);
		int queried_range_size=1<<l32, query_index=l32;
		// 		printf("GetRMQ: left: %d, right: %d, range_size: %d, l32: %d, queried_range_size: %d, query_index: %d\n",
		// 			left, right, range_size, l32, queried_range_size, query_index);
		// 		fflush(stdout);
		return BASE::Select(min_indexes[query_index][left], min_indexes[query_index][right-queried_range_size+1]);
	}
	void Clear()
	{
		min_indexes.clear();
	}
};

/// ---------------------------------------

struct cTreeNode
{
	vector<int> to;
};

vector<cTreeNode> tree_nodes;

// LCA

struct cLCAHelperData
{
	int node_index;
	int depth;
	cLCAHelperData(int node_index, int depth): node_index(node_index), depth(depth) {}
};

vector<cLCAHelperData> lca_helper_data;
vector<int> node_index_to_lca_helper_index;

class cLCAHelperComperator
{
protected:
	int Select(int ai, int bi) const
	{
		return lca_helper_data[ai].depth<lca_helper_data[bi].depth?ai:bi;
	}
};

tRangeMinimumQuery<cLCAHelperComperator> lca_rmq;

void InitLCA_Recursive(int root_index, int from, int depth) // Euler tour, nodes
{
	node_index_to_lca_helper_index[root_index]=(int)lca_helper_data.size();
	lca_helper_data.emplace_back(root_index, depth);
	for(auto to: tree_nodes[root_index].to)
	{
		if(to!=from)
		{
			InitLCA_Recursive(to, root_index, depth+1);
			lca_helper_data.emplace_back(root_index, depth);
		}
	}
}

void InitLCA(vector<int> root_indeces)
{
	lca_helper_data.reserve(tree_nodes.size()*2);
	node_index_to_lca_helper_index.clear();
	node_index_to_lca_helper_index.resize(tree_nodes.size());
	for(auto root_index: root_indeces)
	{
		InitLCA_Recursive(root_index, -1, 0);
	}
	lca_rmq.Clear();
	lca_rmq.Init((int)lca_helper_data.size());
}

int GetLCA(int node_a_index, int node_b_index)
{
	return lca_helper_data[lca_rmq.GetRMQ(node_index_to_lca_helper_index[node_a_index], node_index_to_lca_helper_index[node_b_index])].node_index;
}
