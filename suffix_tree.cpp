class cPackedSuffixTree
{
	std::string mString;
	struct cEdge
	{
		int from, length, ni;
	};
	struct cNode
	{
		cEdge edges[26];
	};
	std::vector<cNode> nodes;
	int node_count=0;
	void Build();
public:
	cPackedSuffixTree()
	{
		nodes.resize(1'000'000); // <-------------------------------------------------------------- CHECK THIS    (memory for speed)
	}
	template<class T>
	void Init(T &&String) // also acts as "Reset" -> can be called multiple times
	{
		mString=std::forward<T>(String);
		Build();
	}
};

void cPackedSuffixTree::Build()
{
	node_count=1;
	auto ResetNode=[this](int ni) { auto &node=nodes[ni]; for(int i=0; i<26; ++i) node.edges[i].from=-1; };
	auto MakeSureNodeExists=[this](int ni) { if(ni>nodes.size()-1) nodes.resize(ni+5'000); };
	MakeSureNodeExists(0);
	ResetNode(0);

	int to=(int)mString.size()-1;
	int from=0;

	for(int f=to; f>=from; --f)
	{
		int ni=0;
		for(int p=f; p<=to; )
		{
			auto &node=nodes[ni];
			cEdge &edge=node.edges[mString[p]-'a'];
			if(edge.from==-1)
			{
				edge.from=p;
				edge.length=to-p+1;
				edge.ni=-1;
				break;
			}
			int new_edge_length=1;
			for(; new_edge_length<=edge.length&&mString[p+new_edge_length]==mString[edge.from+new_edge_length]; ++new_edge_length);
			if(new_edge_length>=edge.length)
			{
				p+=edge.length;
				ni=edge.ni;
				if(ni==-1&&p<=to)
				{
					int new_node_index=node_count;
					edge.ni=new_node_index;
					++node_count;
					MakeSureNodeExists(new_node_index);
					auto &new_node=nodes[new_node_index];
					ResetNode(new_node_index);
					auto &new_edge=new_node.edges[mString[p]-'a'];
					new_edge.from=p;
					new_edge.length=to-p+1;
					new_edge.ni=-1;
					break;
				}
				continue;
			}
			p+=new_edge_length;

			///// split the old edge in two:
			//
			// create the new node to be inserted:
			int new_node_index=node_count;
			++node_count;
			MakeSureNodeExists(new_node_index);
			auto &new_node=nodes[new_node_index];
			ResetNode(new_node_index);
			// update the old edge's length, and link it to the new node
			int original_length=edge.length;
			edge.length=new_edge_length;
			int original_ni=edge.ni;
			edge.ni=new_node_index;
			// add an edge to the new node that will be the second part of the original edge
			int remainder_of_old_edge_from=edge.from+new_edge_length;
			auto &remainder_of_old_edge=new_node.edges[mString[remainder_of_old_edge_from]-'a'];
			remainder_of_old_edge.ni=original_ni;
			remainder_of_old_edge.from=remainder_of_old_edge_from;
			remainder_of_old_edge.length=original_length-new_edge_length;
			//
			///// done splitting
			//
			// add the remainder of the new string to a new edge
			auto &edge_for_new_string=new_node.edges[mString[p]-'a'];
			edge_for_new_string.from=p;
			edge_for_new_string.length=to-p+1;
			edge_for_new_string.ni=-1;
			break;
		}
	}
}



////////////////////// OLD C++:


char text[400001];
int text_length;

class cPackedSuffixTree
{
	struct cEdge
	{
		int from, length, ni;
	};
	struct cNode
	{
		cEdge edges[27];
		void Reset() { for(int i=0; i<27; ++i) edges[i].from=-1; }
	};
	std::vector<cNode> nodes;
	int node_count;
	void Build();
	void MakeSureNodeExists(int ni)
	{
		if(ni>nodes.size()-1) nodes.resize(ni+5000);
	}
	void ResetNode(int ni)
	{
		nodes[ni].Reset();
	}
public:
	cPackedSuffixTree()
	{
		nodes.resize(10000); // <-------------------------------------------------------------- CHECK THIS    (memory for speed)
	}
	void Init() // also acts as "Reset" -> can be called multiple times
	{
		Build();
	}

	void Solve();
};

void cPackedSuffixTree::Build()
{
	node_count=1;
	ResetNode(0);

	int to=text_length;
	text[to]='z'+1;
	int from=0;

	for(int f=to; f>=from; --f)
	{
		int ni=0;
		for(int p=f; p<=to; )
		{
			MakeSureNodeExists(node_count);
			cNode &node=nodes[ni];
			cEdge &edge=node.edges[text[p]-'a'];
			if(edge.from==-1)
			{
				edge.from=p;
				edge.length=to-p+1;
				edge.ni=-1;
				break;
			}
			int new_edge_length=1;
			for(; new_edge_length<=edge.length&&text[p+new_edge_length]==text[edge.from+new_edge_length]; ++new_edge_length);
			if(new_edge_length>=edge.length)
			{
				p+=edge.length;
				ni=edge.ni;
				if(ni==-1&&p<=to)
				{
					int new_node_index=node_count;
					edge.ni=new_node_index;
					++node_count;
//					MakeSureNodeExists(new_node_index);
					cNode &new_node=nodes[new_node_index];
					ResetNode(new_node_index);
					cEdge &new_edge=new_node.edges[text[p]-'a'];
					new_edge.from=p;
					new_edge.length=to-p+1;
					new_edge.ni=-1;
					break;
				}
				continue;
			}
			p+=new_edge_length;

			///// split the old edge in two:
			//
			// create the new node to be inserted:
			int new_node_index=node_count;
			++node_count;
//			MakeSureNodeExists(new_node_index);
			cNode &new_node=nodes[new_node_index];
			ResetNode(new_node_index);
			// update the old edge's length, and link it to the new node
			int original_length=edge.length;
			edge.length=new_edge_length;
			int original_ni=edge.ni;
			edge.ni=new_node_index;
			// add an edge to the new node that will be the second part of the original edge
			int remainder_of_old_edge_from=edge.from+new_edge_length;
			cEdge &remainder_of_old_edge=new_node.edges[text[remainder_of_old_edge_from]-'a'];
			remainder_of_old_edge.ni=original_ni;
			remainder_of_old_edge.from=remainder_of_old_edge_from;
			remainder_of_old_edge.length=original_length-new_edge_length;
			//
			///// done splitting
			//
			// add the remainder of the new string to a new edge
			cEdge &edge_for_new_string=new_node.edges[text[p]-'a'];
			edge_for_new_string.from=p;
			edge_for_new_string.length=to-p+1;
			edge_for_new_string.ni=-1;
			break;
		}
	}
}
