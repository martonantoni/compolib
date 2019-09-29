

enum class eHeapStype { Min, Max };

template<class T, eHeapStype HeapStype=eHeapStype::Max>
class tHeap
{
	std::vector<T> mItems; 
	static size_t GetParentIndex(size_t i) { return (i-1)/2; }
	static size_t GetLeftChildIndex(size_t i) { return (i*2)+1; }
	static size_t GetRightChildIndex(size_t i) { return GetLeftChildIndex(i)+1; }
	static bool IsStronger(const T &a, const T &b) { return HeapStype==eHeapStype::Max?a>b:a<b;	}
	size_t BubbleUp(size_t i)
	{
		while(i) // at most until it is at the root
		{
			auto ParentIndex=GetParentIndex(i);
			if(IsStronger(mItems[i],mItems[ParentIndex]))
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
			bool IsRightBiggerThanLeft=RightChildIndex<mItems.size()&&IsStronger(mItems[RightChildIndex],mItems[LeftChildIndex]);
			auto &ItemToCompare=mItems[IsRightBiggerThanLeft?RightChildIndex:LeftChildIndex];
			if(IsStronger(ItemToCompare,mItems[i]))
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
	bool empty() const { return mItems.empty();	}

	auto begin() { return mItems.begin(); }
	auto end() { return mItems.end(); }
	auto begin() const { return mItems.begin(); }
	auto end() const { return mItems.end(); }
};
