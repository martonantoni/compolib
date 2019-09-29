// https://www.hackerrank.com/challenges/quadrant-queries

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

const int tab32[32] = {
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

template<class T, class P>
class tSegmentTree: public P
{
	std::vector<T> mItems; // the root has index 1
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
		cTraversalData(size_t i, int left_i, int right_i, int left, int right): i(i), left_i(left_i), right_i(right_i), left(left), right(right) {}
		cTraversalData Left() const { return cTraversalData(GetLeftChildIndex(i), left_i, (left_i+right_i)/2, left, right); }
		cTraversalData Right() const { return cTraversalData(GetRightChildIndex(i), (left_i+right_i)/2+1, right_i, left, right); }
	};
	cTraversalData CreateTraversalData(int left, int right) const { return cTraversalData(1, 0, mBottomRowSize-1, left, right); }
	template<class F> void InternalForInterval(const cTraversalData &TraversalData, int mirror_x_counter, int mirror_y_counter, F Func);
	template<class F> void InternalUpdateRange(const cTraversalData &TraversalData, F Func);
	template<class F> void InternalUpdate(size_t i, int left_i, int right_i, int i_to_update, F Func);
public:
	template<class IF> void Init(int Size, IF InitItemsFunction);
	// InitItemsFunction's signature:
	// [](T *Items, int Size)

	template<class F> void UpdateRange(int left, int right, F UpdateFunc);  // Experimental, slow, should use lazy instead
	template<class F> void Update(int i, F UpdateFunc);
	template<class F> void ForInterval(int left, int right, F Func); // both inclusive

///////////////////////////////////////////
// functions required in P:
//
// void MergerFunction(T &result, const T &sub_a, const T &sub_b);
//	  sub_a and sub_b need to be merged into the result
};


template<class T, class P>
template<class IF>
void tSegmentTree<T, P>::Init(int Size, IF InitItemsFunction)
{
	mSize=Size;
	int SizeRequired=2*((int)ones_up_till_highest_bit(Size)+1);
	mItems.resize(SizeRequired);
	mFirstSingleItemIndex=SizeRequired/2;
	mBottomRowSize=SizeRequired/2;
	InitItemsFunction(mItems.data()+mFirstSingleItemIndex, Size);
	for(size_t i=mFirstSingleItemIndex-1; i>0; --i)
	{
		P::MergerFunction(mItems[i], mItems[GetLeftChildIndex(i)], mItems[GetRightChildIndex(i)]);
	}
}

template<class T, class P>
template<class F>
void tSegmentTree<T, P>::InternalForInterval(const cTraversalData &TraversalData, int mirror_x_counter, int mirror_y_counter, F Func)
{
	if(TraversalData.left_i>=TraversalData.left&&TraversalData.right_i<=TraversalData.right)
	{
		Func(mItems[TraversalData.i], mirror_x_counter, mirror_y_counter);
		return;
	}
	int middle=(TraversalData.left_i+TraversalData.right_i)/2;
	auto &item=mItems[TraversalData.i];
	if(middle>=TraversalData.left)
		InternalForInterval(TraversalData.Left(), mirror_x_counter+item.mirror_child_on_x_counter, mirror_y_counter+item.mirror_child_on_y_counter, Func);
	if(middle<TraversalData.right)
		InternalForInterval(TraversalData.Right(), mirror_x_counter+item.mirror_child_on_x_counter, mirror_y_counter+item.mirror_child_on_y_counter, Func);
}

template<class T, class P>
template<class F>
void tSegmentTree<T, P>::ForInterval(int left, int right, F Func) // both inclusive
{
	InternalForInterval(CreateTraversalData(left, right), 0, 0, Func);
}

template<class T, class P>
template<class F>
void tSegmentTree<T, P>::InternalUpdateRange(const cTraversalData &TraversalData, F Func)
{
	if(TraversalData.left_i>=TraversalData.left&&TraversalData.right_i<=TraversalData.right)
	{
		Func(mItems[TraversalData.i]);
		return;
	}
	int middle=(TraversalData.left_i+TraversalData.right_i)/2;
	if(middle>=TraversalData.left)
		InternalUpdateRange(TraversalData.Left(), Func);
	if(middle<TraversalData.right)
		InternalUpdateRange(TraversalData.Right(), Func);
	P::MergerFunction(mItems[TraversalData.i], mItems[GetLeftChildIndex(TraversalData.i)], mItems[GetRightChildIndex(TraversalData.i)]);
}

template<class T, class P>
template<class F>
void tSegmentTree<T, P>::UpdateRange(int left, int right, F UpdateFunc)
{
	InternalUpdateRange(CreateTraversalData(left, right), UpdateFunc);
}

template<class T, class P>
template<class F>
void tSegmentTree<T, P>::InternalUpdate(size_t i, int left_i, int right_i, int i_to_update, F Func)
{
	if(left_i==right_i)
	{
		ASSERT(left_i==i_to_update);
		Func(mItems[i]);
		return;
	}
	int middle=(left_i+right_i)/2;
	if(middle>=i_to_update)
		InternalUpdate(GetLeftChildIndex(i), left_i, middle, i_to_update, Func);
	if(middle<i_to_update)
		InternalUpdate(GetRightChildIndex(i), middle+1, right_i, i_to_update, Func);
	P::MergerFunction(mItems[i], mItems[GetLeftChildIndex(i)], mItems[GetRightChildIndex(i)]);

}

template<class T, class P>
template<class F>
void tSegmentTree<T, P>::Update(int i, F UpdateFunc)
{
	InternalUpdate(1, 0, mBottomRowSize-1, i, UpdateFunc);
}




///-----------------------------------------------------------------------------

struct cSegmentData
{
	int c[4]= { 0 };
	int mirror_child_on_x_counter=0, mirror_child_on_y_counter=0;
	void MirrorOnX()
	{
		swap(c[0], c[3]); 
		swap(c[1], c[2]);
	}
	void MirrorOnY()
	{
		swap(c[0], c[1]); 
		swap(c[2], c[3]);
	}
};

class cSegmentTreeBase
{
protected:
	void MergerFunction(cSegmentData &result, const cSegmentData &sub_a, const cSegmentData &sub_b)
	{
		for(int i=0; i<4; ++i)
			result.c[i]=sub_a.c[i]+sub_b.c[i];
		if(result.mirror_child_on_x_counter&1)
			result.MirrorOnX();
		if(result.mirror_child_on_y_counter&1)
			result.MirrorOnY();
	}
};

const int tr[4]= { 1, 2, 4, 3 };

void Solve()
{

	RI(n);
	vector<char> ps(n);
	for(auto &p: ps)
	{
		RII(x, y);
		p=(x<0?1:0)|(y<0?2:0);
	}
	///////////////////////

	tSegmentTree<cSegmentData, cSegmentTreeBase> seg_tree;
	seg_tree.Init(n, [&ps](cSegmentData *Items, int Size)
	{
		for(int i=0; i<Size; ++i)
		{
			Items[i].c[tr[ps[i]]-1]=1;
		}
	});

	///////////////////////////////
	RI(q);
	char buff[3];
	for(int i=0; i<q; ++i)
	{
		scanf("%s", buff);
		RII(l, h);
		--l; --h;
		switch(buff[0])
		{
		case 'X':
			{
				seg_tree.UpdateRange(l, h, [](cSegmentData &Data) 
				{ 
					Data.MirrorOnX(); 
					++Data.mirror_child_on_x_counter;
				});
				break;
			}
		case 'Y':
			{
				seg_tree.UpdateRange(l, h, [](cSegmentData &Data) 
				{ 
					Data.MirrorOnY(); 
					++Data.mirror_child_on_y_counter;
				});
				break;
			}
		case 'C':
			{
				int c[5]= { 0 };
				seg_tree.ForInterval(l, h, [&c](cSegmentData Data, int mirror_x, int mirror_y) 
				{ 
					if(mirror_x&1)
						Data.MirrorOnX();
					if(mirror_y&1)
						Data.MirrorOnY();
					for(int i=0; i<4; ++i) c[i]+=Data.c[i]; 
				});
				printf("%d %d %d %d\n", c[0], c[1], c[2], c[3]);
				break;
			}
		}
	}
}

int main()
{
	int t=1;
	//	scanf("%d", &t);
	for(int tc=1; tc<=t; ++tc)
	{
		Solve();
	}
}
