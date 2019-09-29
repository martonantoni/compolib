// https://www.hackerrank.com/challenges/kth-ancestor


const int max_log_n=17; // actually this is log2(max_of_n)+1

struct cNode
{
	int ancestors[max_log_n]= { 0 };
	vector<int> children;
	void ClearAncestors()
	{
		fill(ancestors, ancestors+max_log_n, 0);
	}
};

cNode nodes[1'000'01];

vector<int> node_stack;

void BuildAncestorTable(int sub_root)
{
	node_stack.emplace_back(sub_root);
	while(!node_stack.empty())
	{
		sub_root=node_stack.back();
		node_stack.pop_back();
		cNode &node=nodes[sub_root];
		for(int i=1; i<max_log_n&&node.ancestors[i-1]; ++i)
		{
			node.ancestors[i]=nodes[node.ancestors[i-1]].ancestors[i-1];
		}
		for(auto c: node.children)
			node_stack.emplace_back(c);
	}
}

int GetKthAncestor(int node_index, int k)
{
	for(int i=max_log_n-1, mask=1<<(max_log_n-1); i>=0; --i, mask>>=1)
	{
		if(k&mask)
		{
			node_index=nodes[node_index].ancestors[i];
			if(node_index==0)
				return 0;
		}
	}
	return node_index;
}

void ClearChildren(int root)
{
	node_stack.emplace_back(root);
	while(!node_stack.empty())
	{
		root=node_stack.back();
		node_stack.pop_back();
		for(auto c: nodes[root].children)
			node_stack.emplace_back(c);
		nodes[root].children.clear();
		nodes[root].ClearAncestors();
	}
}

void SolveQueries()
{
	RI(q);
	for(int i=0; i<q; ++i)
	{
		RI(t);
		switch(t)
		{
		case 0: // add node
			{
				RII(parent, child);
//				nodes[child].ClearAncestors();
				nodes[child].ancestors[0]=parent;
				nodes[parent].children.emplace_back(child);
				BuildAncestorTable(child);
				break;
			}
		case 1: // remove node
			{
				RI(x);
				nodes[x].ClearAncestors();
				nodes[x].children.clear();
				break;
			}
		case 2:
			{
				RII(x, k);
				printf("%d\n", GetKthAncestor(x, k));
				break;
			}
		}
	}
}

void Solve()
{
	RI(n);
	int root=0;
	for(int i=0; i<n; ++i)
	{
		RII(child, parent);
		if(parent==0)
		{
			root=child;
//			nodes[child].ClearAncestors();
		}
		else
		{
//			nodes[child].ClearAncestors();
			nodes[child].ancestors[0]=parent;
			nodes[parent].children.emplace_back(child);
		}
	}
	BuildAncestorTable(root);
	SolveQueries();
	ClearChildren(root);
}

int Init()
{
	RI(t);
	return t;
}

int main()
{
#ifdef _DEBUG
	//	TestRMQ();
#endif
	//	std::ios::sync_with_stdio(false);
	int t=Init();
	//	int t=1;
	//	scanf("%d", &t);
	for(int tc=1; tc<=t; ++tc)
	{
		Solve();
	}
}