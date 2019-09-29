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


struct cSegmentData
{
	int lazy_add=0;
	int max_value=0;
	/////////////////////////////
	ll first_index, length=1;
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
	int InternalQueryInterval(const cTraversalData &TraversalData);
	void InternalIncreaseRange(const cTraversalData &TraversalData, int added);
	void InternalSetValue(const cTraversalData &TraversalData, int value);
	void PushLazy(const cTraversalData &TraversalData, int lazy_add);
public:
	void Init(int Size);

	void IncreaseRange(int left, int right, int added);
	int QueryInterval(int left, int right);

	void SetValue(int item, int value);

	void MergerFunction(cSegmentData &result, const cSegmentData &sub_a, const cSegmentData &sub_b)
	{
		result.max_value=max(sub_a.max_value+sub_a.lazy_add, sub_b.max_value+sub_b.lazy_add);
	}
};

void cSegmentTree::Init(int Size)
{
	mSize=Size;
	int SizeRequired=Size==1?2:2*((int)ones_up_till_highest_bit(Size-1)+1);
	mItems.resize(SizeRequired);
	mFirstSingleItemIndex=SizeRequired/2;
	mBottomRowSize=SizeRequired/2;

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
// 	for(size_t i=mFirstSingleItemIndex-1; i>0; --i)
// 	{
// 		MergerFunction(mItems[i], mItems[GetLeftChildIndex(i)], mItems[GetRightChildIndex(i)]);
// 	}
}

int cSegmentTree::InternalQueryInterval(const cTraversalData &TraversalData)
{
	auto &item=mItems[TraversalData.i];
	if(TraversalData.left_i>=TraversalData.left&&TraversalData.right_i<=TraversalData.right)
	{
		return item.max_value+item.lazy_add;
	}
	if(item.lazy_add)
	{
		PushLazy(TraversalData.Left(), item.lazy_add);
		PushLazy(TraversalData.Right(), item.lazy_add);
		item.lazy_add=0;
	}
	int middle=(TraversalData.left_i+TraversalData.right_i)/2;
	
	int result=0;

	if(middle>=TraversalData.left)
		result=InternalQueryInterval(TraversalData.Left());
	if(middle<TraversalData.right)
		result=max(result, InternalQueryInterval(TraversalData.Right()));
	return result;
}

int cSegmentTree::QueryInterval(int left, int right) // both inclusive
{
	return InternalQueryInterval(CreateTraversalData(left, right));
}


///                   Increase


void cSegmentTree::InternalIncreaseRange(const cTraversalData &TraversalData, int add)
{
	auto &item=mItems[TraversalData.i];
	if(TraversalData.left_i>=TraversalData.left&&TraversalData.right_i<=TraversalData.right)
	{
		item.lazy_add+=add;
		return;
	}
	if(item.lazy_add)
	{
		PushLazy(TraversalData.Left(), item.lazy_add);
		PushLazy(TraversalData.Right(), item.lazy_add);
		item.lazy_add=0;
	}
	int middle=(TraversalData.left_i+TraversalData.right_i)/2;
	if(middle>=TraversalData.left)
		InternalIncreaseRange(TraversalData.Left(), add);
	if(middle<TraversalData.right)
		InternalIncreaseRange(TraversalData.Right(), add);
	MergerFunction(mItems[TraversalData.i], mItems[GetLeftChildIndex(TraversalData.i)], mItems[GetRightChildIndex(TraversalData.i)]);
}

void cSegmentTree::IncreaseRange(int left, int right, int add)
{
	InternalIncreaseRange(CreateTraversalData(left, right), add);
}


void cSegmentTree::PushLazy(const cTraversalData &TraversalData, int lazy_add)
{
	auto &item=mItems[TraversalData.i];
	item.lazy_add+=lazy_add;
}

void cSegmentTree::InternalSetValue(const cTraversalData &TraversalData, int value)
{
	auto &item=mItems[TraversalData.i];
	if(TraversalData.left_i>=TraversalData.left&&TraversalData.right_i<=TraversalData.right)
	{
		item.max_value=value;
		item.lazy_add=0;
		return;
	}
	if(item.lazy_add)
	{
		PushLazy(TraversalData.Left(), item.lazy_add);
		PushLazy(TraversalData.Right(), item.lazy_add);
		item.lazy_add=0;
	}
	int middle=(TraversalData.left_i+TraversalData.right_i)/2;
	if(middle>=TraversalData.left)
		InternalSetValue(TraversalData.Left(), value);
	if(middle<TraversalData.right)
		InternalSetValue(TraversalData.Right(), value);
	MergerFunction(mItems[TraversalData.i], mItems[GetLeftChildIndex(TraversalData.i)], mItems[GetRightChildIndex(TraversalData.i)]);
}

void cSegmentTree::SetValue(int item, int value)
{
	InternalSetValue(CreateTraversalData(item, item), value);
}
