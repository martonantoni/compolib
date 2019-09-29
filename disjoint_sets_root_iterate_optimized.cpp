https://www.hackerrank.com/challenges/kundu-and-tree

#include <unordered_map>
#include <memory>
#include <tuple>

using ull=unsigned long long;
using ll=long long;
static const ll mod=1'000'000'007;

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
	int empty() const { return mFirst==ALMOST_NULL; }
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

//#define OPTIMIZE_ROOT_ITERATION

struct cIntrusiveDisjointSetsItemBase
{
	size_t mParentIndex=-1;
	int mRank=0;
	bool IsRoot() const { return mParentIndex==-1; }
	void SetJoined(cIntrusiveDisjointSetsItemBase &JoiningSet) {}
};

template<class T> // T must have cIntrusiveDisjointSetsItemBase as parent class
class cIntrusiveDisjointSets
{
	std::vector<T> mItems;
#ifdef OPTIMIZE_ROOT_ITERATION
	tIntrusiveList<T> mRoots;
#endif
	void JoinIntoSet(size_t JoiningSet, size_t JoinedSet)
	{
		mItems[JoiningSet].mParentIndex=JoinedSet;
	// challenge specific code for union:
		mItems[JoinedSet].SetJoined(mItems[JoiningSet]);
#ifdef OPTIMIZE_ROOT_ITERATION
		mRoots.remove(&mItems[JoiningSet]);
#endif
	}
public:
	void Init(size_t NumberOfItems)
	{
		mItems.resize(NumberOfItems);
#ifdef OPTIMIZE_ROOT_ITERATION
		for(auto &Item: mItems)
			mRoots.push_back(&Item);
#endif
	}
	void Init(std::vector<T> &&Items)
	{
		mItems=std::move(Items);
#ifdef OPTIMIZE_ROOT_ITERATION
		for(auto &Item: mItems)
			mRoots.push_back(&Item);
#endif
	}
	size_t GetRoot(size_t i) const
	{
		auto &Item=mItems[i];
		if(Item.mParentIndex==-1)
			return i;
		// path compression:
		auto RootIndex=GetRoot(Item.mParentIndex);
		const_cast<size_t &>(Item.mParentIndex)=RootIndex;
		return RootIndex;
	}
	void Union(size_t a, size_t b)
	{
		auto a_root_i=GetRoot(a), b_root_i=GetRoot(b);
		if(a_root_i==b_root_i)
			return;
		if(mItems[a_root_i].mRank>mItems[b_root_i].mRank)
		{
			JoinIntoSet(b_root_i, a_root_i);
		}
		else
		{
			JoinIntoSet(a_root_i, b_root_i);
			if(mItems[a_root_i].mRank==mItems[b_root_i].mRank)
				++mItems[b_root_i].mRank;
		}
	}
	template<class F>
	void ForEachRoot(const F &f)
	{
#ifdef OPTIMIZE_ROOT_ITERATION
		for(auto i=mRoots.begin(), iend=mRoots.end(); i!=iend; ++i)
			f(**i);
#else
		for(auto &Item: mItems)
			if(Item.IsRoot())
				f(Item);
#endif
	}

	T &operator[](size_t i) { return mItems[i]; }
	const T &operator[](size_t i) const { return mItems[i]; }
	auto begin() { return mItems.begin(); }
	auto end() { return mItems.end(); }
};


struct cItem: public cIntrusiveDisjointSetsItemBase
#ifdef OPTIMIZE_ROOT_ITERATION
	, public tIntrusiveListItemBase<cItem>
#endif
{
	ll mGroupSize=1;
	void SetJoined(cItem &JoiningSet)
	{
		mGroupSize+=JoiningSet.mGroupSize;
	}
};


void Init()
{
}

ll set_size_sum=0;
ll cube_sum=0;
ll square_sum=0;

void Solve()
{
	ll n;
	scanf("%lld", &n);
	cIntrusiveDisjointSets<cItem> sets;
	sets.Init(n);
	char buf[5];
	for(int i=1; i<n; ++i)
	{
		int a, b;
		scanf("%d %d %s", &a, &b, buf);
		if(buf[0]=='b')
		{
			sets.Union(a-1, b-1);
		}
	}
	set_size_sum=n;
	sets.ForEachRoot([n](cItem &Item)
	{
		ll t=Item.mGroupSize*Item.mGroupSize;
		t%=mod;
		ll t2=t*Item.mGroupSize;
		t2%=mod;
		cube_sum+=t2;
		cube_sum%=mod;
		ll t3=t*(n-Item.mGroupSize);
		t3%=mod;
		square_sum+=t3;
		square_sum%=mod;
	});
	ll sum_cubed=n*n;
	sum_cubed%=mod;
	sum_cubed*=n;
	sum_cubed%=mod;
	square_sum*=3;
	square_sum%=mod;
	ll result=sum_cubed-square_sum-cube_sum+mod+mod;
	result%=mod;
	result*=166666668;
	result%=mod;
	printf("%lld", result);
}


int main()
{
	Init();
	int t=1;
//	scanf("%d", &t);
	for(int tc=1; tc<=t; ++tc)
	{
		Solve();
	}
}
