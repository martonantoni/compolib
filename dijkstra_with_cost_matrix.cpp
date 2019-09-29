// https://www.hackerrank.com/challenges/dijkstrashortreach

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <set>
#include <tuple>
#include <cstring>
#include <deque>

#undef max
#undef min

#ifndef _MSC_VER 
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

#ifdef DEBUG
#define ASSERT(x) if(!x) _asm int 3;
#else
#define ASSERT(x)
#endif

using namespace std;
using namespace std::string_literals;
using ull=unsigned long long;
using ll=long long;
constexpr ll mod=1'000'000'007;

#define RI(var_name) int var_name; scanf("%d", &var_name);
#define RIV(var_name, size) vector<int> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%d", &item_of_##var_name);
#define RII(var_name1, var_name2) int var_name1, var_name2; scanf("%d %d", &var_name1, &var_name2);
#define RIII(var_name1, var_name2, var_name3) int var_name1, var_name2, var_name3; scanf("%d %d %d", &var_name1, &var_name2, &var_name3);
#define RL(var_name) ll var_name; scanf("%lld", &var_name);
#define RLV(var_name, size) vector<ll> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%lld", &item_of_##var_name);
#define RLL(var_name1, var_name2) ll var_name1, var_name2; scanf("%lld %lld", &var_name1, &var_name2);
#define RLLL(var_name1, var_name2, var_name3) ll var_name1, var_name2, var_name3; scanf("%lld %lld %lld", &var_name1, &var_name2, &var_name3);
#define ALL(cont) cont.begin(), cont.end()
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class eHeapStype { Min, Max };

template<class T, eHeapStype HeapStype=eHeapStype::Max>
class tHeap
{
	std::vector<T> mItems;
	static size_t GetParentIndex(size_t i) { return (i-1)/2; }
	static size_t GetLeftChildIndex(size_t i) { return (i*2)+1; }
	static size_t GetRightChildIndex(size_t i) { return GetLeftChildIndex(i)+1; }
	static bool IsStronger(const T &a, const T &b) { return HeapStype==eHeapStype::Max?a>b:a<b; }
	size_t BubbleUp(size_t i)
	{
		while(i) // at most until it is at the root
		{
			auto ParentIndex=GetParentIndex(i);
			if(IsStronger(mItems[i], mItems[ParentIndex]))
			{
				std::swap(mItems[i], mItems[ParentIndex]);
				i=ParentIndex;
			}
			else
				break;
		}
		return i;
	}
	void FixHeap(size_t i)
	{
		for(;;)
		{
			auto LeftChildIndex=GetLeftChildIndex(i);
			if(LeftChildIndex>=mItems.size())
				break;
			auto &LeftChild=mItems[LeftChildIndex];
			auto RightChildIndex=GetRightChildIndex(i);
			bool IsRightBiggerThanLeft=RightChildIndex<mItems.size()&&IsStronger(mItems[RightChildIndex], mItems[LeftChildIndex]);
			auto &ItemToCompare=mItems[IsRightBiggerThanLeft?RightChildIndex:LeftChildIndex];
			if(IsStronger(ItemToCompare, mItems[i]))
			{
				std::swap(ItemToCompare, mItems[i]);
				i=IsRightBiggerThanLeft?RightChildIndex:LeftChildIndex;
			}
			else
				break;
		}
	}
public:
	void push(T Item)
	{
		size_t i=mItems.size();
		mItems.emplace_back(Item);
		BubbleUp(i);
	}
	void erase(size_t i)
	{
		if(i==mItems.size()-1)
		{
			mItems.pop_back();
			return;
		}
		std::swap(mItems.back(), mItems[i]);
		mItems.pop_back();

		if(i!=BubbleUp(i))
			return;
		FixHeap(i);
	}
	void erase(typename std::vector<T>::const_iterator i) { erase(i-mItems.begin()); }
	void heapify(std::vector<T> &&Items)
	{
		mItems=std::move(Items);
		if(mItems.size()<=1)
			return;
		size_t i=GetParentIndex(mItems.size()-1);
		for(;;)
		{
			FixHeap(i);
			if(i==0)
				break;
			--i;
		}
	}
	const T &front() const { return mItems[0]; }
	T pop_front()
	{
		T t(std::move(mItems[0]));
		erase(0);
		return t;
	}
	size_t size() const { return mItems.size(); }
	bool empty() const { return mItems.empty(); }

	auto begin() { return mItems.begin(); }
	auto end() { return mItems.end(); }
	auto begin() const { return mItems.begin(); }
	auto end() const { return mItems.end(); }
};

vector<vector<int>> edge_costs; 
vector<vector<int>> targets_from_nodes;

int GetCost(int from, int to)
{
	return from>to?edge_costs[to][from-to]:edge_costs[from][to-from];
}

bool SetCost(int from, int to, int cost)
{
	auto &stored_cost=from>to?edge_costs[to][from-to]:edge_costs[from][to-from];
	if(stored_cost==0)
	{
		stored_cost=cost;
		return true;
	}
	stored_cost=min(stored_cost, cost);
	return false;
}


void RunDijkstra(int from)
{
	struct cEdgePos
	{
		int i;
		int w;
		bool operator<(const cEdgePos &other) const { return w<other.w; }
		bool operator>(const cEdgePos &other) const { return w>other.w; }
		cEdgePos(int i, int w): i(i), w(w) {}
	};
	tHeap<cEdgePos, eHeapStype::Min> frontier;
	vector<int> best_w(edge_costs.size(), numeric_limits<int>::max());
	best_w[from]=0;
	frontier.push({ from, 0 });
	while(!frontier.empty())
	{
		auto f=frontier.pop_front();
		for(auto target: targets_from_nodes[f.i])
		{
			int new_w=f.w+GetCost(f.i, target);  /// <---- cost calculation
			if(new_w<best_w[target])
			{
				frontier.push({ target, new_w });
				best_w[target]=new_w;
			}
		}
	}
	for(auto &cost: best_w)
	{
		if(cost)
			printf("%d ", cost==numeric_limits<int>::max()?-1:cost);
	}
	printf("\n");
}

void Solve()
{
	RII(n, e);
	edge_costs.clear();
	targets_from_nodes.clear();
	edge_costs.resize(n);
	int vec_size=n;
	for(auto &x: edge_costs)
	{
		x.resize(vec_size, 0);
		--vec_size;
	}
	targets_from_nodes.resize(n);
	for(int i=0; i<e; ++i)
	{
		RIII(f, t, w);
		--f; --t;

		if(SetCost(f, t, w))
		{
			targets_from_nodes[f].emplace_back(t);
			targets_from_nodes[t].emplace_back(f);
		}
	}
	RI(s);

	RunDijkstra(s-1);
}

int main()
{
	int t=1;
	scanf("%d", &t);
	for(int tc=1; tc<=t; ++tc)
	{
		Solve();
	}
}