template<class T>
class tIndexingHeap
{
	std::vector<T> mItems;
	std::vector<size_t> mOriginalIndexToHeapIndex;
	static size_t GetParentIndex(size_t i) { return (i-1)/2; }
	static size_t GetLeftChildIndex(size_t i) { return (i*2)+1; }
	static size_t GetRightChildIndex(size_t i) { return GetLeftChildIndex(i)+1; }
	static bool IsStronger(const T &a, const T &b) { return a<b; }
	size_t BubbleUp(size_t i)
	{
		while(i) // at most until it is at the root
		{
			auto ParentIndex=GetParentIndex(i);
			if(IsStronger(mItems[i], mItems[ParentIndex]))
			{
				std::swap(mItems[i], mItems[ParentIndex]);
				mOriginalIndexToHeapIndex[mItems[i].original_index]=i;
				mOriginalIndexToHeapIndex[mItems[ParentIndex].original_index]=ParentIndex;
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
			size_t ItemToCompareIndex=IsRightBiggerThanLeft?RightChildIndex:LeftChildIndex;
			auto &ItemToCompare=mItems[ItemToCompareIndex];
			if(IsStronger(ItemToCompare, mItems[i]))
			{
				std::swap(ItemToCompare, mItems[i]);
				mOriginalIndexToHeapIndex[ItemToCompare.original_index]=ItemToCompareIndex;
				mOriginalIndexToHeapIndex[mItems[i].original_index]=i;
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
		mOriginalIndexToHeapIndex[Item.original_index]=i;
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
		mOriginalIndexToHeapIndex[mItems.back().original_index]=mItems.size()-1;
		mOriginalIndexToHeapIndex[mItems[i].original_index]=i;
		mItems.pop_back();

		if(i!=BubbleUp(i))
			return;
		FixHeap(i);
	}
	void erase_by_original_index(size_t original_index)
	{
		auto heap_index=mOriginalIndexToHeapIndex[original_index];
		while(heap_index) // at most until it is at the root
		{
			auto ParentIndex=GetParentIndex(heap_index);
			std::swap(mItems[heap_index], mItems[ParentIndex]);
			mOriginalIndexToHeapIndex[mItems[heap_index].original_index]=heap_index;
			mOriginalIndexToHeapIndex[mItems[ParentIndex].original_index]=ParentIndex;
			heap_index=ParentIndex;
		}
		pop_front();
	}
	void heapify(std::vector<T> &&Items)
	{
		mItems=std::move(Items);
		mOriginalIndexToHeapIndex.resize(mItems.size());
		if(mItems.size()<=1)
		{
			mOriginalIndexToHeapIndex[mItems[0].original_index]=0;
			return;
		}
		size_t i=GetParentIndex(mItems.size()-1);
		for(;;)
		{
			FixHeap(i);
			if(i==0)
				break;
			--i;
		}
		for(size_t i=0, iend=mItems.size(); i!=iend;++i)
		{
			mOriginalIndexToHeapIndex[mItems[i].original_index]=i;
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

void HeapTest()
{
	struct cHeapItem
	{
		int value=0;
		size_t original_index;
		bool operator<(const cHeapItem &other) const { return value<other.value; }
	};
	tIndexingHeap<cHeapItem> heap;
	vector<cHeapItem> items(100);
	for(int i=0; i<100; ++i)
	{
		items[i].value=i*5%100+i/20;
		items[i].original_index=i;
	}
	heap.heapify(move(items));
	heap.erase_by_original_index(10);
	while(!heap.empty())
	{
		auto item=heap.pop_front();
		printf("%d\n", item.value);
	}
}




////////////////////// ALTERNATE VERSION:
// made for https://www.hackerrank.com/contests/w32/challenges/balls-and-boxes


template<class T>
class tIndexingHeap
{
	std::vector<T> mItems;
	std::vector<size_t> mBoxIndexToHeapIndex;
	static size_t GetParentIndex(size_t i) { return (i-1)/2; }
	static size_t GetLeftChildIndex(size_t i) { return (i*2)+1; }
	static size_t GetRightChildIndex(size_t i) { return GetLeftChildIndex(i)+1; }
	static bool IsStronger(const T &a, const T &b) { return a<b; }
	size_t BubbleUp(size_t i)
	{
		while(i) // at most until it is at the root
		{
			auto ParentIndex=GetParentIndex(i);
			if(IsStronger(mItems[i], mItems[ParentIndex]))
			{
				std::swap(mItems[i], mItems[ParentIndex]);
				mBoxIndexToHeapIndex[mItems[i].box_index]=i;
				mBoxIndexToHeapIndex[mItems[ParentIndex].box_index]=ParentIndex;
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
			size_t ItemToCompareIndex=IsRightBiggerThanLeft?RightChildIndex:LeftChildIndex;
			auto &ItemToCompare=mItems[ItemToCompareIndex];
			if(IsStronger(ItemToCompare, mItems[i]))
			{
				std::swap(ItemToCompare, mItems[i]);
				mBoxIndexToHeapIndex[ItemToCompare.box_index]=ItemToCompareIndex;
				mBoxIndexToHeapIndex[mItems[i].box_index]=i;
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
		mBoxIndexToHeapIndex[Item.box_index]=i;
		BubbleUp(i);
	}
	void erase(size_t i)
	{
		if(i==mItems.size()-1)
		{

			mBoxIndexToHeapIndex[mItems.back().box_index]=numeric_limits<size_t>::max(); // setting it to invalid as it gets removed
			mItems.pop_back();

			return;
		}
		std::swap(mItems.back(), mItems[i]);
		mBoxIndexToHeapIndex[mItems[i].box_index]=i;

		mBoxIndexToHeapIndex[mItems.back().box_index]=numeric_limits<size_t>::max(); // setting it to invalid as it gets removed
		mItems.pop_back();

		if(i!=BubbleUp(i))
			return;
		FixHeap(i);
	}
	const T &get_by_box_index(int box_index) const
	{
		auto heap_index=mBoxIndexToHeapIndex[box_index];
		return mItems[heap_index];
	}
	bool is_valid_box_index(int box_index) const
	{
		return mBoxIndexToHeapIndex[box_index]!=numeric_limits<size_t>::max();
	}
	void erase_by_box_index(size_t box_index)
	{
		auto heap_index=mBoxIndexToHeapIndex[box_index];
		while(heap_index) // bubble up to the top
		{
			auto ParentIndex=GetParentIndex(heap_index);
			std::swap(mItems[heap_index], mItems[ParentIndex]);
			mBoxIndexToHeapIndex[mItems[heap_index].box_index]=heap_index;
			mBoxIndexToHeapIndex[mItems[ParentIndex].box_index]=ParentIndex;
			heap_index=ParentIndex;
		}
		pop_front();
	}
	void heapify(std::vector<T> &&Items, int number_of_boxes)
	{
		mItems=std::move(Items);
		mBoxIndexToHeapIndex.resize(number_of_boxes, numeric_limits<size_t>::max());
		if(mItems.size()<=1)
		{
			if(mItems.size()==1)
			{
				mBoxIndexToHeapIndex[mItems[0].box_index]=0;
			}
			return;
		}
		size_t i=GetParentIndex(mItems.size()-1);
		for(;;)
		{
			FixHeap(i);
			if(i==0)
				break;
			--i;
		}
		for(size_t i=0, iend=mItems.size(); i!=iend;++i)
		{
			mBoxIndexToHeapIndex[mItems[i].box_index]=i;
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


