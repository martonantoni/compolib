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


#define ALMOST_NULL ((T *)((char *)-1))

template<class T, int ListCount=1> struct tIntrusiveListItemBase
{
	struct cIntrusiveListItemNeighbours
	{
		T *mNext, *mPrev;
		cIntrusiveListItemNeighbours(): mNext(NULL), mPrev(NULL) {}
		~cIntrusiveListItemNeighbours() { ASSERT(mNext==NULL&&mPrev==NULL); }
	};
	cIntrusiveListItemNeighbours mIntrusiveListItemNeighbours[ListCount];
	virtual ~tIntrusiveListItemBase() {}
	bool IsInIntrusiveList(int ListIndex=0) const { return mIntrusiveListItemNeighbours[ListIndex].mNext!=NULL; }
};

template<class T, int ListIndex=0> struct tDefaultIntrusiveListAccessor
{
	static T *&Next(T *Item) { return Item->mIntrusiveListItemNeighbours[ListIndex].mNext; }
	static T *&Prev(T *Item) { return Item->mIntrusiveListItemNeighbours[ListIndex].mPrev; }
	static const T *Next(const T *Item) { return Item->mIntrusiveListItemNeighbours[ListIndex].mNext; }
	static const T *Prev(const T *Item) { return Item->mIntrusiveListItemNeighbours[ListIndex].mPrev; }
	static bool IsInList(const T *Item) { return Next(Item)!=NULL; }
};

template<class T, int ListIndex=0> class tIntrusiveList
{
	typedef tDefaultIntrusiveListAccessor<T, ListIndex> Accessor;
	T *mFirst, *mLast;
	size_t mItemCount;
	void SanityCheck()
	{
		size_t CountedItems=0; T *PrevItem=mFirst;
		for(T *Item=mFirst; Item!=ALMOST_NULL; Item=Accessor::Next(Item))
		{
			++CountedItems;
			PrevItem=Item;
		}
		ASSERT(CountedItems==mItemCount);
		ASSERT(PrevItem==mLast);
	}
public:
	tIntrusiveList(): mFirst(ALMOST_NULL), mLast(ALMOST_NULL), mItemCount(0) {}
	tIntrusiveList(const tIntrusiveList &) = delete;
	tIntrusiveList(tIntrusiveList &&Source);
	tIntrusiveList &operator=(const tIntrusiveList &) = delete;
	tIntrusiveList &operator=(tIntrusiveList &&Source);
	static const T *GetTerminator() { return (const T *)ALMOST_NULL; }
	static bool IsInList(const T *Item) { return Accessor::Next(Item)!=NULL; }
	void push_back(T *Item);
	void push_front(T *Item);
	T *pop_back();
	T *pop_front();
	T *back() const { return mLast; }
	T *front() const { return mFirst; }
	static T *get_next(T *Item) { return Accessor::Next(Item)==GetTerminator()?NULL:Accessor::Next(Item); }
	static T *get_prev(T *Item) { return Accessor::Prev(Item)==GetTerminator()?NULL:Accessor::Prev(Item); }
	static const T *get_next(const T *Item) { return Accessor::Next(Item)==GetTerminator()?NULL:Accessor::Next(Item); }
	static const T *get_prev(const T *Item) { return Accessor::Prev(Item)==GetTerminator()?NULL:Accessor::Prev(Item); }
	bool empty() const { return mFirst==ALMOST_NULL; }
	size_t size() const { return mItemCount; }
	void clear();
	class const_iterator
	{
	public:
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef const T* value_type;
		typedef ptrdiff_t difference_type;
		typedef value_type* pointer;
		typedef value_type& reference;

		const T *mItem;
		friend class tIntrusiveList<T>;
		const_iterator(const T *pItem): mItem(pItem) {}
		const_iterator(const const_iterator &Source): mItem(Source.mItem) {}
		const_iterator(): mItem(ALMOST_NULL) {}
		void operator++() { mItem=Accessor::Next(mItem); }
		const T *operator*() const { return mItem; }
		const T *operator->() const { return mItem; }

		int operator==(const const_iterator &Other) const { return mItem==Other.mItem; }
		int operator!=(const const_iterator &Other) const { return mItem!=Other.mItem; }
	};
	class iterator: public const_iterator
	{
	public:
		typedef T* value_type;
		typedef value_type* pointer;
		typedef value_type& reference;

		iterator(T *pItem): const_iterator(pItem) {}
		iterator(const iterator &Source): const_iterator(Source) {}
		iterator() {}
		void operator++() { this->mItem=Accessor::Next(this->mItem); }
		T *operator*() { return (T *)this->mItem; }
		T *operator->() { return (T *)this->mItem; }

		int operator==(const iterator &Other) const { return this->mItem==Other.mItem; }
		int operator!=(const iterator &Other) const { return this->mItem!=Other.mItem; }
	};
	iterator begin() { return iterator(mFirst); }
	iterator end() { return iterator(ALMOST_NULL); }
	const_iterator begin() const { return const_iterator(mFirst); }
	const_iterator end() const { return const_iterator(ALMOST_NULL); }
	iterator erase(iterator &i);
	iterator insert(iterator &i, T *NewItem); // Item must not be in an other allocless list
	void remove(T *Item);
	void ExchangeLists(tIntrusiveList<T, ListIndex> &OtherList);
	void splice(iterator i, tIntrusiveList<T, ListIndex> &OtherList);
};

template<class T, int ListIndex>
tIntrusiveList<T, ListIndex>::tIntrusiveList(tIntrusiveList &&Source)
	: mFirst(Source.mFirst)
	, mLast(Source.mLast)
	, mItemCount(Source.mItemCount)
{
	Source.mFirst=Source.mLast=ALMOST_NULL;
	Source.mItemCount=0;
}

template<class T, int ListIndex>
auto tIntrusiveList<T, ListIndex>::operator=(tIntrusiveList &&Source) -> tIntrusiveList &
{
	mFirst=Source.mFirst;
	mLast=Source.mLast;
	mItemCount=Source.mItemCount;
	Source.mFirst=Source.mLast=ALMOST_NULL;
	Source.mItemCount=0;
	return *this;
}

template<class T, int ListIndex> void tIntrusiveList<T, ListIndex>::push_back(T *Item)
{
	ASSERT(Accessor::Next(Item)==NULL&&Accessor::Prev(Item)==NULL);
	++mItemCount;
	Accessor::Next(Item)=ALMOST_NULL;
	if(mLast!=ALMOST_NULL)
	{
		Accessor::Prev(Item)=mLast;
		Accessor::Next(mLast)=Item;
		mLast=Item;
	}
	else
	{
		mFirst=mLast=Item;
		Accessor::Prev(Item)=ALMOST_NULL;
	}
}

template<class T, int ListIndex> void tIntrusiveList<T, ListIndex>::push_front(T *Item)
{
	ASSERT(Accessor::Next(Item)==NULL&&Accessor::Prev(Item)==NULL);
	++mItemCount;
	Accessor::Prev(Item)=ALMOST_NULL;
	if(mFirst!=ALMOST_NULL)
	{
		Accessor::Next(Item)=mFirst;
		Accessor::Prev(mFirst)=Item;
		mFirst=Item;
	}
	else
	{
		mFirst=mLast=Item;
		Accessor::Next(Item)=ALMOST_NULL;
	}
}

template<class T, int ListIndex> T *tIntrusiveList<T, ListIndex>::pop_back()
{
	--mItemCount;
	T *Item=mLast;
	if(Item!=ALMOST_NULL)
	{
		mLast=Accessor::Prev(mLast);
		if(mLast!=ALMOST_NULL)
			Accessor::Next(mLast)=ALMOST_NULL;
		else
			mFirst=ALMOST_NULL;
		Accessor::Prev(Item)=Accessor::Next(Item)=NULL;
	}
	return Item;
}

template<class T, int ListIndex> T *tIntrusiveList<T, ListIndex>::pop_front()
{
	--mItemCount;
	T *Item=mFirst;
	if(Item!=ALMOST_NULL)
	{
		mFirst=Accessor::Next(mFirst);
		if(mFirst!=ALMOST_NULL)
			Accessor::Prev(mFirst)=ALMOST_NULL;
		else
			mLast=ALMOST_NULL;
		Accessor::Prev(Item)=Accessor::Next(Item)=NULL;
	}
	return Item;
}

template<class T, int ListIndex> void tIntrusiveList<T, ListIndex>::remove(T *Item)
{
	if(Item==NULL||Accessor::Prev(Item)==NULL)
	{
		// Invalid item: NULL or not in an allocless list.
		return;
	}
	--mItemCount;
	if(Accessor::Prev(Item)!=ALMOST_NULL)
		Accessor::Next(Accessor::Prev(Item))=Accessor::Next(Item);
	else
		mFirst=Accessor::Next(Item);
	if(Accessor::Next(Item)!=ALMOST_NULL)
		Accessor::Prev(Accessor::Next(Item))=Accessor::Prev(Item);
	else
		mLast=Accessor::Prev(Item);
	Accessor::Prev(Item)=Accessor::Next(Item)=NULL;
}

template<class T, int ListIndex> typename tIntrusiveList<T, ListIndex>::iterator tIntrusiveList<T, ListIndex>::erase(iterator &i)
{
	iterator NextIterator(Accessor::Next(*i));
	remove(*i);
	return NextIterator;
}

template<class T, int ListIndex> typename tIntrusiveList<T, ListIndex>::iterator tIntrusiveList<T, ListIndex>::insert(iterator &i, T *NewItem)
{
	ASSERT(Accessor::Next(NewItem)==NULL&&Accessor::Prev(NewItem)==NULL);
	++mItemCount;
	T *IteratorItem=(T *)i.mItem;
	Accessor::Next(NewItem)=IteratorItem;
	if(IteratorItem!=ALMOST_NULL)
	{
		if(Accessor::Prev(IteratorItem)!=ALMOST_NULL)
			Accessor::Next(Accessor::Prev(IteratorItem))=NewItem;
		else
			mFirst=NewItem;
		Accessor::Prev(NewItem)=Accessor::Prev(IteratorItem);
		Accessor::Prev(IteratorItem)=NewItem;
	}
	else  // this will be the last item
	{
		Accessor::Prev(NewItem)=mLast;
		if(mLast!=ALMOST_NULL)
			Accessor::Next(mLast)=NewItem;
		else
			mFirst=NewItem;
		mLast=NewItem;
	}
	return iterator(NewItem);
}

template<class T, int ListIndex> void tIntrusiveList<T, ListIndex>::clear()
{
	for(T *Item=mFirst; Item!=ALMOST_NULL;)
	{
		T *NextItem=Accessor::Next(Item);
		Accessor::Next(Item)=Accessor::Prev(Item)=NULL;
		Item=NextItem;
	}
	mFirst=mLast=ALMOST_NULL;
	mItemCount=0;
}

template<class T, int ListIndex> void tIntrusiveList<T, ListIndex>::ExchangeLists(tIntrusiveList<T, ListIndex> &OtherList)
{
	std::swap(mFirst, OtherList.mFirst);
	std::swap(mLast, OtherList.mLast);
	std::swap(mItemCount, OtherList.mItemCount);
}

template<class T, int ListIndex> void tIntrusiveList<T, ListIndex>::splice(iterator i, tIntrusiveList<T, ListIndex> &OtherList)
{
	if(OtherList.mItemCount==0)
		return;
	ASSERT(&OtherList!=this);
	if(mItemCount==0)
	{
		std::swap(mFirst, OtherList.mFirst);
		std::swap(mLast, OtherList.mLast);
		std::swap(mItemCount, OtherList.mItemCount);
		return;
	}
	T *IteratorItem=(T *)i.mItem;
	if(IteratorItem!=ALMOST_NULL)
	{
		// inserting before IteratorItem
		Accessor::Prev(OtherList.mFirst)=Accessor::Prev(IteratorItem);
		if(Accessor::Prev(IteratorItem))
		{
			Accessor::Next(Accessor::Prev(IteratorItem))=OtherList.mFirst;
		}
		else
		{
			mFirst=OtherList.mFirst;
		}
		Accessor::Prev(IteratorItem)=OtherList.mLast;
		Accessor::Next(OtherList.mLast)=IteratorItem;
	}
	else
	{
		// inserting at the end
		Accessor::Next(mLast)=OtherList.mFirst;
		Accessor::Prev(OtherList.mFirst)=mLast;
		mLast=OtherList.mLast;
	}
	mItemCount+=OtherList.mItemCount;
	OtherList.mItemCount=0;
	OtherList.mFirst=OtherList.mLast=ALMOST_NULL;
}

#undef ALMOST_NULL


template<class T>
struct tFibonacciHeapItem: public tIntrusiveListItemBase<tFibonacciHeapItem<T>>, public T
{
	using cItem=tFibonacciHeapItem<T>;
	tIntrusiveList<cItem> mChildren;
	cItem *mParent=nullptr;
	int mDegree=0; // number of children
	bool mIsMarked=false;
	
	template<class ...Ts> tFibonacciHeapItem(Ts &&...ts): T(std::forward<Ts>(ts)...) {}
};

template<class T>
class tFibonacciHeap
{
public:
	using cItem=tFibonacciHeapItem<T>;
private:
	vector<cItem *> mCleanupHelper; // kept as member so it does not get reallocated every time its needed
	tIntrusiveList<cItem> mRoots; // minimum item is at front
	size_t mSize=0;
	void LinkRoots(cItem *RootA, cItem *RootB);
	void RemoveFromParent(cItem *Node);
	void AddChild(cItem *Parent, cItem *Child);
	void Promote(cItem *Item);
	void Cleanup();
public:
	void Insert(cItem *Item);
	cItem *GetMinimum() { return mRoots.front(); }
	void RemoveMinimum();
//	void RemoveItem(cItem *Item);
	void ItemPriorityChanged(cItem *Item);
	bool empty() const { return mRoots.empty(); }
	size_t size() const { return mSize; }
};

template<class T> void tFibonacciHeap<T>::Insert(cItem *Item)
{
	if(mRoots.empty()||(*Item)<(*mRoots.front()))
		mRoots.push_front(Item);
	else
		mRoots.push_back(Item);
	Item->mParent=nullptr;
	Item->mIsMarked=false;
	Item->mDegree=0;
	++mSize;
}

template<class T> void tFibonacciHeap<T>::RemoveFromParent(cItem *Node)
{
	if(Node->mParent)
	{
		Node->mChildren.remove(Node);
		-Node->mParent->mDegree;
		Node->mParent->mMarked=true;
	}
}

template<class T> void tFibonacciHeap<T>::AddChild(cItem *Parent, cItem *Child)
{
	++Parent->mDegree;
	Parent->mChildren.push_back(Child);
	Child->mParent=Parent;
}

template<class T> void tFibonacciHeap<T>::LinkRoots(cItem *RootA, cItem *RootB)
{
	ASSERT(RootA->mParent==nullptr&&RootB->mParent==nullptr);
	if((*RootA)<(*RootB)||mRoots.front()==RootB) // == is in case of RootA<=RootB
	{
		// RootA will be the parent of RootB
		mRoots.remove(RootB);
		AddChild(RootA, RootB);
	}
	else
	{
		mRoots.remove(RootA);
		AddChild(RootB, RootA);
	}
}

template<class T> void tFibonacciHeap<T>::Cleanup()
{
// helper functions:
	auto SetB=[this](int Degree, cItem *Item)
	{
		if(mCleanupHelper.size()<=Degree)
			mCleanupHelper.resize(Degree+1, nullptr);
		mCleanupHelper[Degree]=Item;
	};
	auto GetB=[this](int Degree)
	{
		return Degree<mCleanupHelper.size()?mCleanupHelper[Degree]:nullptr;
	};
// Cleanup:
	cItem *NewMin=mRoots.front();
	fill(ALL(mCleanupHelper), nullptr);
	for(auto i=mRoots.begin(); i!=mRoots.end();)
	{
		cItem *v=*i;
		++i;
		v->mParent=nullptr;
		v->mIsMarked=false;
		if(*v<*NewMin)
			NewMin=v;
		// LinkDupes:
		cItem *w=GetB(v->mDegree);
		while(w)
		{
			SetB(v->mDegree, nullptr);
			if(*w<*v||w==NewMin)
			{
				swap(w, v);
			}
			ASSERT(w!=NewMin);
			mRoots.remove(w);
			AddChild(v, w);
			w=GetB(v->mDegree);
		}
		SetB(v->mDegree, v);
	}

	if(NewMin!=mRoots.front())
	{
		mRoots.remove(NewMin);
		mRoots.push_front(NewMin);
	}
}

template<class T> void tFibonacciHeap<T>::RemoveMinimum()
{
	if(mRoots.empty())
		return;
	ASSERT(mSize);
	--mSize;
// real stuff comes here:
	cItem *MinimumItem=mRoots.front();
	mRoots.remove(MinimumItem);
	if(!MinimumItem->mChildren.empty())
	{
		mRoots.splice(mRoots.end(), MinimumItem->mChildren);
	}
	if(mRoots.empty())
		return;
// Cleanup:
	Cleanup();
}

template<class T> void tFibonacciHeap<T>::Promote(cItem *Item)
{
	Item->mIsMarked=false;
	if(Item->mParent)
	{
		cItem *Parent=Item->mParent;
		Item->mParent=nullptr;
		Parent->mChildren.remove(Item);
		--Parent->mDegree;
		if(*Item<*mRoots.front())
			mRoots.push_front(Item);
		else
			mRoots.push_back(Item);
		if(Parent->mIsMarked)
			Promote(Parent);
		else
			Parent->mIsMarked=true;
	}
}

template<class T> void tFibonacciHeap<T>::ItemPriorityChanged(cItem *Item)
{
	if(Item->mParent)
	{
		if(*Item<*Item->mParent)
		{
			Promote(Item);			
		}
	}
	else
	{
		if(Item!=mRoots.front()&&*Item<*mRoots.front())
		{
			mRoots.remove(Item);
			mRoots.push_front(Item);
		}
		else if(Item==mRoots.front())
		{
			Cleanup();
		}
	}
}

///---------------------------------------------------------------------------------------------------------------------------------

struct cMyHeapItem
{
	int x;
	cMyHeapItem(int x): x(x) {}
	cMyHeapItem() {}

	bool operator<(const cMyHeapItem &Other)
	{
		return x<Other.x;
	}
};

void Solve()
{
	tFibonacciHeap<cMyHeapItem> h;
	int n, k;
	scanf("%d %d", &n, &k);
	vector<tFibonacciHeap<cMyHeapItem>::cItem> items(n);
	for(auto &item: items)
	{
		scanf("%d", &item.x);
		h.Insert(&item);
	}
	int op=0;
	while(h.GetMinimum()->x<k)
	{
		if(h.size()<2)
		{
			printf("-1");
			while(!h.empty())
				h.RemoveMinimum();
			return;
		}
		auto ns=h.GetMinimum();
		h.RemoveMinimum();
		auto ns2=h.GetMinimum();
		h.RemoveMinimum();
		ns->x+=ns2->x*2;
		h.Insert(ns);
		++op;
	}
	printf("%d", op);
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