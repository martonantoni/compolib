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
#include <map>
#include <random>

#undef max
#undef min

#ifdef __GNUC__ 
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

#ifdef _DEBUG
#define ASSERT(x) if(!(x)) __debugbreak()
#else
char *crash_please=(char *)42;
#define ASSERT(x) 
//if(!(x)) { printf("%s failed",#x); *crash_please=33; }
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
		//		printf("--- Elapsed %llu ms in %s ---\n", (unsigned long long)Elapsed.count(), mName);
	}
};

using namespace std;
using namespace std::string_literals;
using ull=unsigned long long;
using ll=long long;
constexpr ll mod=1'000'000'007;

template<class I> auto rev(I i) { return std::reverse_iterator<I>(i); }

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
#define FOR(var, max_value) for(int var=0;var<max_value;++var)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::mt19937 TreapRandomGenerator(42424242);
std::uniform_int_distribution<int> TreapPriorityGenerator(0, std::numeric_limits<int>::max());


using cIntervalType = int;

namespace IntervalTree
{
	struct cNode
	{
		cIntervalType low, high;
	// add augmentation / payload here:
		int mMaxEndpoint;
		void MaintainAugmentation()
		{
			mMaxEndpoint=max(high,
				max(mLeft?mLeft->mMaxEndpoint:numeric_limits<cIntervalType>::min(),
					mRight?mRight->mMaxEndpoint:numeric_limits<cIntervalType>::min()));
		}
	// construction:
		cNode(int low, int high): low(low), high(high), mMaxEndpoint(high) {}
	// do not touch:
		int mPriority=TreapPriorityGenerator(TreapRandomGenerator);
		cNode *mLeft=nullptr, *mRight=nullptr;
		int mSize=1;
		void FixSize()
		{
			mSize=(mLeft?mLeft->mSize:0)+(mRight?mRight->mSize:0)+1;
			MaintainAugmentation();
		}
	};
	bool CompareKeys(cIntervalType a, cIntervalType b)
	{
		return a<b;
	}

	std::pair<cNode *, cNode *> Split(cNode *Root, cIntervalType Key);
	cNode *Merge(cNode *Left, cNode *Right);
	cNode *Insert(cNode *Root, cNode *NewNode);
	cNode *Remove(cNode *Root, cNode *NodeToRemove); // does not delete NodeToRemove
	size_t size(cNode *Root) { return Root?Root->mSize:0; }
	cNode *FindIntersectingInterval(cNode *root, int low, int high);

	std::pair<cNode *, cNode *> Split(cNode *Root, cIntervalType Key)
	{
		if(!Root)
			return make_pair(nullptr, nullptr);
		if(CompareKeys(Key, Root->low))
		{
			auto SplitLeft=Split(Root->mLeft, Key);
			Root->mLeft=SplitLeft.second;
			Root->FixSize();
			return make_pair(SplitLeft.first, Root);
		}
		else
		{
			auto SplitRight=Split(Root->mRight, Key);
			Root->mRight=SplitRight.first;
			Root->FixSize();
			return make_pair(Root, SplitRight.second);
		}
	}

	cNode *Merge(cNode *Left, cNode *Right)
	{
		if(!Left||!Right)
			return Left?Left:Right;
		if(Left->mPriority>Right->mPriority)
		{   // higher priority will be nearer the root
			// so in this case Left will be above Right
			Left->mRight=Merge(Left->mRight, Right);
			Left->FixSize();
			return Left;
		}
		else
		{
			// in this case Rigth will be above Left
			Right->mLeft=Merge(Left, Right->mLeft);
			Right->FixSize();
			return Right;
		}
	}

	cNode *Insert(cNode *Root, cNode *NewNode)
	{
		if(!Root)
		{
			NewNode->mLeft=NewNode->mRight=nullptr;
			return NewNode;
		}
		if(NewNode->mPriority>Root->mPriority)
		{
			// NewNode must be above Root to maintain heap property
			tie(NewNode->mLeft, NewNode->mRight)=Split(Root, NewNode->low);
			NewNode->FixSize();
			return NewNode;
		}
		if(CompareKeys(NewNode->low, Root->low))
			Root->mLeft=Insert(Root->mLeft, NewNode);
		else
			Root->mRight=Insert(Root->mRight, NewNode);
		Root->FixSize();
		return Root;
	}

	cNode *Remove(cNode *Root, cNode *NodeToRemove)
	{
		if(!Root)
			return Root;
		if(Root==NodeToRemove)
		{
			auto NewRoot=Merge(Root->mLeft, Root->mRight);
			NewRoot->FixSize();
			return NewRoot;
		}
		if(CompareKeys(NodeToRemove->low, Root->low))
		{
			Root->mLeft=Remove(Root->mLeft, NodeToRemove);
			return Root;
		}
		else if(CompareKeys(Root->low, NodeToRemove->low))
		{
			Root->mRight=Remove(Root->mRight, NodeToRemove);
			return Root;
		}
		else
			return nullptr; // shouldn't happen
	}
}

int result=0;

namespace IntervalTree
{
	// Interval tree specific stuff:
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	cNode *FindIntersectingInterval(cNode *root, int low, int high)
	{
		if(!root)
			return root;
		if(high>=root->low&&low<=root->high)
			return root;
		if(!root->mLeft)
			return FindIntersectingInterval(root->mRight, low, high);
		if(root->mMaxEndpoint<low)
			return FindIntersectingInterval(root->mRight, low, high);
		return FindIntersectingInterval(root->mLeft, low, high);
	}

	void HandleAllIntersecting(cNode *root, int low, int high)
	{
		if(!root)
			return;
		if(high>=root->low&&low<=root->high)
		{
/// intersecting interval handling code:
			++result;
/// end of intersecting interval handler code
		}
		if(root->mLeft&&root->mLeft->mMaxEndpoint>=low)
			HandleAllIntersecting(root->mLeft, low, high);
		if(root->mRight&&root->low<=high)
			HandleAllIntersecting(root->mRight, low, high);
	}
}


using namespace IntervalTree;

// https://www.hackerrank.com/challenges/x-and-his-shots

void Solve() 
{
	RII(n, m);
	vector<cNode> nodes;
	nodes.reserve(n); // preallocate nodes (avoid alloc calls, and more cache friendly)
	cNode *root=nullptr;
	for(int i=0; i<n; ++i)
	{
		RII(low, high);
		nodes.emplace_back(low, high);
		root=Insert(root, &nodes.back());
	}
	for(int i=0; i<m; ++i)
	{
		RII(low, high);
		HandleAllIntersecting(root, low, high);
	}
	printf("%d", result);
}

int Init()
{
//	RI(t);
	int t=1;
	return t;
}

int main()
{
	//	std::ios::sync_with_stdio(false);
	int t=Init();
	for(int tc=1; tc<=t; ++tc)
	{
//		printf("Case #%d: ", tc);
		Solve();
	}
}