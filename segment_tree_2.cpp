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
	ll sum=0;
	ll value=42; // only if leaf
	bool is_all_zero=true;
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
	ll InternalQueryInterval(const cTraversalData &TraversalData);
	void InternalIncreaseRange(const cTraversalData &TraversalData);
public:
	void Init(int Size);

	void IncreaseRange(int left, int right);

	ll QueryInterval(int left, int right);

	void InternalSetValue(const cTraversalData &TraversalData, ll value);
	void SetValue(int item, ll value);

	void MergerFunction(cSegmentData &result, const cSegmentData &sub_a, const cSegmentData &sub_b)
	{
		result.sum=sub_a.sum+sub_b.sum;
		result.is_all_zero = sub_a.is_all_zero&&sub_b.is_all_zero;
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
	for(int i=(int)mFirstSingleItemIndex; i<mFirstSingleItemIndex+Size; ++i)
	{
		RI(x);
		mItems[i].value=x;
		if(x<=40)
		{
			mItems[i].sum=facts[x];
			mItems[i].is_all_zero=false;
		}
		else
		{
			mItems[i].sum=0;
			mItems[i].is_all_zero=true;
		}
	}


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

ll cSegmentTree::InternalQueryInterval(const cTraversalData &TraversalData)
{
	auto &item=mItems[TraversalData.i];
	if(TraversalData.left_i>=TraversalData.left&&TraversalData.right_i<=TraversalData.right)
	{
		return item.sum;
	}
	if(item.is_all_zero)
	{
		return 0;
	}
	int middle=(TraversalData.left_i+TraversalData.right_i)/2;
	ll result=0;
	if(middle>=TraversalData.left)
		result+=InternalQueryInterval(TraversalData.Left());
	if(middle<TraversalData.right)
		result+=InternalQueryInterval(TraversalData.Right());
	return result%mod;
}

ll cSegmentTree::QueryInterval(int left, int right) // both inclusive
{
	return InternalQueryInterval(CreateTraversalData(left, right));
}


///                   Increase


void cSegmentTree::InternalIncreaseRange(const cTraversalData &TraversalData)
{
	auto &item=mItems[TraversalData.i];
	if(item.length==1) // is leaf
	{
		++item.value;
		if(item.value>=41)
		{
			item.is_all_zero=true;
			item.sum=0;
		}
		else
		{
			item.is_all_zero=false;
			item.sum=facts[item.value];
		}
		return;
	}
	if(item.is_all_zero)
		return;
	int middle=(TraversalData.left_i+TraversalData.right_i)/2;
	if(middle>=TraversalData.left)
		InternalIncreaseRange(TraversalData.Left());
	if(middle<TraversalData.right)
		InternalIncreaseRange(TraversalData.Right());
	MergerFunction(mItems[TraversalData.i], mItems[GetLeftChildIndex(TraversalData.i)], mItems[GetRightChildIndex(TraversalData.i)]);
}

void cSegmentTree::IncreaseRange(int left, int right)
{
	InternalIncreaseRange(CreateTraversalData(left, right));
}



void cSegmentTree::InternalSetValue(const cTraversalData &TraversalData, ll value)
{
	auto &item=mItems[TraversalData.i];
	if(item.length==1) // is leaf
	{
		item.value=value;
		if(item.value>=41)
		{
			item.is_all_zero=true;
			item.sum=0;
		}
		else
		{
			item.is_all_zero=false;
			item.sum=facts[item.value];
		}
		return;
	}
	int middle=(TraversalData.left_i+TraversalData.right_i)/2;
	if(middle>=TraversalData.left)
		InternalSetValue(TraversalData.Left(), value);
	if(middle<TraversalData.right)
		InternalSetValue(TraversalData.Right(), value);
	MergerFunction(mItems[TraversalData.i], mItems[GetLeftChildIndex(TraversalData.i)], mItems[GetRightChildIndex(TraversalData.i)]);
}

void cSegmentTree::SetValue(int item, ll value)
{
	InternalSetValue(CreateTraversalData(item, item), value);
}

