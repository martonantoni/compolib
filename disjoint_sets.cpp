struct cIntrusiveDisjointSetsItemBase
{
	size_t mParentIndex=-1;
	int mRank=0;
	bool IsRoot() const { return mParentIndex==-1; }
	void SetJoined(cIntrusiveDisjointSetsItemBase &JoiningSet) {}
};

template<class T> // T must have cIntrusiveDisjointSetsItemBase as parent class
class tIntrusiveDisjointSets
{
	std::vector<T> mItems;
	size_t mNumberOfRoots=0;
	void JoinIntoSet(size_t JoiningSet, size_t JoinedSet)
	{
		mItems[JoiningSet].mParentIndex=JoinedSet;
		// challenge specific code for union:
		mItems[JoinedSet].SetJoined(mItems[JoiningSet]);
		--mNumberOfRoots;
	}
public:
	tIntrusiveDisjointSets()=default;
	tIntrusiveDisjointSets(tIntrusiveDisjointSets<T> &other)=delete;
	tIntrusiveDisjointSets &operator=(tIntrusiveDisjointSets<T> &other)=delete;
	tIntrusiveDisjointSets &operator=(tIntrusiveDisjointSets<T> &&other)
	{
		if(&other==this)
			return *this;
		mItems=std::move(other.mItems);
		mNumberOfRoots=other.mNumberOfRoots;
		other.mNumberOfRoots=0;
		return *this;
	}
	void Init(size_t NumberOfItems)
	{
		mItems.resize(NumberOfItems);
		mNumberOfRoots=NumberOfItems;
	}
	void Init(std::vector<T> &&Items)
	{
		mItems=std::move(Items);
		mNumberOfRoots=mItems.size();
	}
	int AddItem()
	{
		mItems.emplace_back();
		++mNumberOfRoots;
		return (int)mItems.size()-1;
	}
	template<class U> int AddItem(U &&item)
	{
		mItems.emplace_back(std::forward<U>(item));
		++mNumberOfRoots;
		return (int)mItems.size()-1;
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
		for(auto &Item: mItems)
			if(Item.IsRoot())
				f(Item);
	}
	size_t GetRootCount() const { return mNumberOfRoots; }
	T &operator[](size_t i) { return mItems[i]; }
	const T &operator[](size_t i) const { return mItems[i]; }
	auto begin() { return mItems.begin(); }
	auto end() { return mItems.end(); }
	void clear() { mItems.clear(); mNumberOfRoots=0; }
};

tIntrusiveDisjointSets<cIntrusiveDisjointSetsItemBase> sets;
