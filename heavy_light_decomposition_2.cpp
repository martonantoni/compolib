// https://www.hackerrank.com/challenges/heavy-light-2-white-falcon


#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <string>
#include <set>
#include <tuple>
#include <functional>
#include <cstring>
#include <deque>
#include <array>

#undef max
#undef min

#ifdef __GNUC__ 
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

#ifdef _DEBUG
#define ASSERT(x) if(!(x)) __debugbreak()
#else
#define ASSERT(x) 
#endif

#include <chrono>

class cLogPerformance_Guard
{
	std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime=std::chrono::high_resolution_clock::now();
	const char *mName;
public:
	cLogPerformance_Guard(const char *Name): mName(Name) {}
	~cLogPerformance_Guard()
	{
		auto EndTime=std::chrono::high_resolution_clock::now();
		auto Elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(EndTime-mStartTime);
//		printf("--- Elapsed %llu ms in %s ---\n", Elapsed.count(), mName);
	}
};


using namespace std;
using namespace std::string_literals;
using ull=unsigned long long;
using ll=long long;
constexpr ll mod=1'000'000'007;

#define RI(var_name) int var_name; scanf("%d", &var_name);
#define RIV(var_name, size) vector<int> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%d", &item_of_##var_name);
#define RII(var_name1, var_name2) int var_name1, var_name2; scanf("%d %d", &var_name1, &var_name2);
#define RIII(var_name1, var_name2, var_name3) int var_name1, var_name2, var_name3; scanf("%d %d %d", &var_name1, &var_name2, &var_name3);
#define RIIII(var_name1, var_name2, var_name3, var_name4) int var_name1, var_name2, var_name3, var_name4; scanf("%d %d %d %d", &var_name1, &var_name2, &var_name3, &var_name4);
#define RL(var_name) ll var_name; scanf("%lld", &var_name);
#define RLV(var_name, size) vector<ll> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%lld", &item_of_##var_name);
#define RLL(var_name1, var_name2) ll var_name1, var_name2; scanf("%lld %lld", &var_name1, &var_name2);
#define RLLL(var_name1, var_name2, var_name3) ll var_name1, var_name2, var_name3; scanf("%lld %lld %lld", &var_name1, &var_name2, &var_name3);
#define RD(var_name) double var_name; scanf("%lf", &var_name);
#define RDV(var_name, size) vector<double> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%d", &item_of_##var_name);
#define RDD(var_name1, var_name2) double var_name1, var_name2; scanf("%lf %lf", &var_name1, &var_name2);
#define RDDD(var_name1, var_name2, var_name3) double var_name1, var_name2, var_name3; scanf("%lf %lf %lf", &var_name1, &var_name2, &var_name3);
#define ALL(cont) cont.begin(), cont.end()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ull isolate_highest_bit(ull x)
{
	x|=x>>1;
	x|=x>>2;
	x|=x>>4;
	x|=x>>8;
	x|=x>>16;
	x|=x>>32;
	return x&~(x>>1);
}

ull ones_up_till_highest_bit(ull x) //   for example 0000'0000'0010'1101 -->  0000'0000'0011'1111
{
	return x==0?0:(((isolate_highest_bit(x)-1ull)<<1ull)|1ull);
}

#define FIX_MOD_AFTER_ADD(x) if(x>=mod) x%=mod; else while(x<0) x=mod+x%mod;

struct cAddedExp
{
	ll a=0ll, b=0ll; // a*x+b
	cAddedExp() {}
	cAddedExp(ll _a, ll _b): a(_a), b(_b) 
	{
		FIX_MOD_AFTER_ADD(a); FIX_MOD_AFTER_ADD(b);
	}
	cAddedExp &operator+=(const cAddedExp &other)
	{
		a+=other.a;
		b+=other.b;
		FIX_MOD_AFTER_ADD(a); FIX_MOD_AFTER_ADD(b);
		return *this;
	}
	cAddedExp operator+(const cAddedExp &other)
	{
		return cAddedExp(a+other.a, b+other.b);
	}
};

struct cSegmentData
{
	cAddedExp added_to_entire;
	ll sum;
	ll first_index, length=1;

	ll CalculateAddedEffect(cAddedExp to_add) const
	{
		return length*(to_add.a*(2*first_index+length-1)+2*to_add.b)/2;
	}
};

class cSegmentTree
{
	std::vector<cSegmentData> mItems; // the root has index 1
	static size_t GetParentIndex(size_t i) { return i/2; }
	static size_t GetLeftChildIndex(size_t i) { return i*2; }
	static size_t GetRightChildIndex(size_t i) { return GetLeftChildIndex(i)+1; }
	int mSize=0;
	int mBottomRowSize=0;
	size_t mFirstSingleItemIndex=0;
	struct cTraversalData
	{
		size_t i;
		int left_i, right_i;
		int left, right;
		cTraversalData(size_t i, int left_i, int right_i, int left, int right): i(i), left_i(left_i), right_i(right_i), left(max(left_i, left)), right(min(right_i, right))
		{
		}
		cTraversalData Left() const
		{
			return cTraversalData(GetLeftChildIndex(i), left_i, (left_i+right_i)/2, left, right);
		}
		cTraversalData Right() const
		{
			return cTraversalData(GetRightChildIndex(i), (left_i+right_i)/2+1, right_i, left, right);
		}
	};
	cTraversalData CreateTraversalData(int left, int right) const { return cTraversalData(1, 0, mBottomRowSize-1, left, right); }
	ll InternalQueryInterval(const cTraversalData &TraversalData, cAddedExp to_add);
	void InternalAddToRange(const cTraversalData &TraversalData, cAddedExp to_add);
public:
	void Init(int Size);

	void AddToRange(int left, int right, cAddedExp to_add);

	ll QueryInterval(int left, int right);

	void MergerFunction(cSegmentData &result, const cSegmentData &sub_a, const cSegmentData &sub_b)
	{
		result.sum=sub_a.sum+sub_b.sum;
		result.sum+=result.CalculateAddedEffect(result.added_to_entire);

		result.sum%=mod;
	}
};

void cSegmentTree::Init(int Size)
{
	mSize=Size;
	int SizeRequired=Size==1?2:2*((int)ones_up_till_highest_bit(Size-1)+1);
	mItems.resize(SizeRequired);
	mFirstSingleItemIndex=SizeRequired/2;
	mBottomRowSize=SizeRequired/2;
// 	InitItemsFunction(mItems.data()+mFirstSingleItemIndex, Size);
	for(int i=(int)mFirstSingleItemIndex; i<SizeRequired; ++i)
	{
		mItems[i].first_index=i-mFirstSingleItemIndex;
	}
	for(int i=(int)mFirstSingleItemIndex+Size; i<SizeRequired; ++i)
	{
		mItems[i]=mItems[mFirstSingleItemIndex+Size-1];
	}
	for(size_t i=mFirstSingleItemIndex-1; i>0; --i)
	{
		mItems[i].length=mItems[GetLeftChildIndex(i)].length*2;
		mItems[i].first_index=mItems[GetLeftChildIndex(i)].first_index;
	}
	for(size_t i=mFirstSingleItemIndex-1; i>0; --i)
	{
		MergerFunction(mItems[i], mItems[GetLeftChildIndex(i)], mItems[GetRightChildIndex(i)]);
	}
}

ll cSegmentTree::InternalQueryInterval(const cTraversalData &TraversalData, cAddedExp to_add)
{
	auto &item=mItems[TraversalData.i];
	if(TraversalData.left_i>=TraversalData.left&&TraversalData.right_i<=TraversalData.right)
	{
		return (item.sum+item.CalculateAddedEffect(to_add))%mod;
	}
	int middle=(TraversalData.left_i+TraversalData.right_i)/2;
	ll result=0;
	if(middle>=TraversalData.left)
		result+=InternalQueryInterval(TraversalData.Left(), to_add+item.added_to_entire);
	if(middle<TraversalData.right)
		result+=InternalQueryInterval(TraversalData.Right(), to_add+item.added_to_entire);
	return result%mod;
}

ll cSegmentTree::QueryInterval(int left, int right) // both inclusive
{
	return InternalQueryInterval(CreateTraversalData(left, right), {});
}


///                   A D D


void cSegmentTree::InternalAddToRange(const cTraversalData &TraversalData, cAddedExp to_add)
{
	auto &item=mItems[TraversalData.i];
	if(TraversalData.left_i>=TraversalData.left&&TraversalData.right_i<=TraversalData.right)
	{
		item.added_to_entire+=to_add;
		item.sum+=item.CalculateAddedEffect(to_add);
		item.sum%=mod;
		return;
	}
	int middle=(TraversalData.left_i+TraversalData.right_i)/2;
	if(middle>=TraversalData.left)
		InternalAddToRange(TraversalData.Left(), to_add);
	if(middle<TraversalData.right)
		InternalAddToRange(TraversalData.Right(), to_add);
	MergerFunction(mItems[TraversalData.i], mItems[GetLeftChildIndex(TraversalData.i)], mItems[GetRightChildIndex(TraversalData.i)]);
}

void cSegmentTree::AddToRange(int left, int right, cAddedExp to_add)
{
	if(right==mSize-1)
	{
		right=mBottomRowSize-1;
	}
	InternalAddToRange(CreateTraversalData(left, right), to_add);
}

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

	cSegmentTree seg_tree;

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
		chain.seg_tree.Init(chain.length);
	}
}

ll GetSumBetweenNodes(int u, int v)
{
	ll result=0;
	for(;;)
	{
		auto &chain_u=chains[tree[u].chain], &chain_v=chains[tree[v].chain];
		int index_in_chain_u=tree[chain_u.root].top_height-tree[u].top_height;
		int index_in_chain_v=tree[chain_v.root].top_height-tree[v].top_height;
		if(tree[u].chain==tree[v].chain)
		{
			if(index_in_chain_u>index_in_chain_v)
				swap(index_in_chain_u, index_in_chain_v);
 			result+=chain_u.seg_tree.QueryInterval(index_in_chain_u, index_in_chain_v);
			result%=mod;
			break;
		}
		if(tree[chain_u.root].depth>tree[chain_v.root].depth)
		{
 			result+=chain_u.seg_tree.QueryInterval(0, index_in_chain_u);
			result%=mod;
			u=tree[chain_u.root].parent;
		}
		else
		{
			result+=chain_v.seg_tree.QueryInterval(0, index_in_chain_v);
			result%=mod;
			v=tree[chain_v.root].parent;
		}
	}
	return result%mod;
}

void UpdateBetweenNodes(int u, int v, ll k)
{
	ll distance=0;
	int u_orig=u, v_orig=v;
	for(;;)
	{
		auto &chain_u=chains[tree[u].chain], &chain_v=chains[tree[v].chain];
		int index_in_chain_u=tree[chain_u.root].top_height-tree[u].top_height;
		int index_in_chain_v=tree[chain_v.root].top_height-tree[v].top_height;
		if(tree[u].chain==tree[v].chain)
		{
			if(index_in_chain_u>index_in_chain_v)
				swap(index_in_chain_u, index_in_chain_v);
			distance+=index_in_chain_v-index_in_chain_u+1;
			break;
		}
		if(tree[chain_u.root].depth>tree[chain_v.root].depth)
		{
			distance+=index_in_chain_u+1;
			u=tree[chain_u.root].parent;
		}
		else
		{
			distance+=index_in_chain_v+1;
			v=tree[chain_v.root].parent;
		}
	}
	u=u_orig, v=v_orig;
	ll m_u=1, m_v=distance;
	for(;;)
	{
		auto &chain_u=chains[tree[u].chain], &chain_v=chains[tree[v].chain];
		int index_in_chain_u=tree[chain_u.root].top_height-tree[u].top_height;
		int index_in_chain_v=tree[chain_v.root].top_height-tree[v].top_height;
		if(tree[u].chain==tree[v].chain)
		{
			if(index_in_chain_u>index_in_chain_v)
			{
				chain_u.seg_tree.AddToRange(index_in_chain_v, index_in_chain_u, { -k, (m_u+index_in_chain_u)*k });
			}
			else
			{
				chain_u.seg_tree.AddToRange(index_in_chain_u, index_in_chain_v, { k, (m_u-index_in_chain_u)*k });
			}
			return;
		}
		if(tree[chain_u.root].depth>tree[chain_v.root].depth)
		{
			chain_u.seg_tree.AddToRange(0, index_in_chain_u, { -k, (m_u+index_in_chain_u)*k });
			m_u+=index_in_chain_u+1;
			u=tree[chain_u.root].parent;
		}
		else
		{
			chain_v.seg_tree.AddToRange(0, index_in_chain_v, { k,  (m_v-index_in_chain_v)*k });
			m_v-=index_in_chain_v+1;
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
			RII(u, v);
			RL(x);
//			--u;
			UpdateBetweenNodes(u, v, x);
// 			auto &node=tree[u];
// 			auto &chain=chains[node.chain];
// 			int index_in_chain=tree[chain.root].top_height-node.top_height;
// 			chain.segment_tree.Update(index_in_chain, [x](auto &item) { item.x=x; });
		}
		else
		{
			RII(u, v);
			ll s=GetSumBetweenNodes(u, v);
			if(s<0||s>=mod)
			{
				int alma=22;
			}
			printf("%lld\n", s);
		}
	}
}

void Init()
{
}

int main()
{
	//	std::ios::sync_with_stdio(false);
	Init();
	int t=1;
//	scanf("%d", &t);
	for(int tc=1; tc<=t; ++tc)
	{
		Solve();
	}
}