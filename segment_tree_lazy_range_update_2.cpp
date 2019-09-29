// https://www.hackerrank.com/challenges/heavy-light-2-white-falcon

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

#define FIX_MOD_AFTER_ADD(x) if(x>=mod) x%=mod; else while(x<0) x=mod+x%mod;

struct cAddedExp
{
	ll a=0ll, b=0ll; // a*x+b
	cAddedExp() {}
	cAddedExp(ll _a, ll _b): a(_a), b(_b) 
	{
		FIX_MOD_AFTER_ADD(a); FIX_MOD_AFTER_ADD(b);
	}
	cAddedExp &operator+=(const cAddedExp &other)
	{
		a+=other.a;
		b+=other.b;
		FIX_MOD_AFTER_ADD(a); FIX_MOD_AFTER_ADD(b);
		return *this;
	}
	cAddedExp operator+(const cAddedExp &other)
	{
		return cAddedExp(a+other.a, b+other.b);
	}
};

struct cSegmentData
{
	cAddedExp added_to_entire;
	ll sum;
	ll first_index, length=1;

	ll CalculateAddedEffect(cAddedExp to_add) const
	{
		return length*(to_add.a*(2*first_index+length-1)+2*to_add.b)/2;
	}
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
	ll InternalQueryInterval(const cTraversalData &TraversalData, cAddedExp to_add);
	void InternalAddToRange(const cTraversalData &TraversalData, cAddedExp to_add);
public:
	void Init(int Size);

	void AddToRange(int left, int right, cAddedExp to_add);

	ll QueryInterval(int left, int right);

	void MergerFunction(cSegmentData &result, const cSegmentData &sub_a, const cSegmentData &sub_b)
	{
		result.sum=sub_a.sum+sub_b.sum;
		result.sum+=result.CalculateAddedEffect(result.added_to_entire);

		result.sum%=mod;
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

ll cSegmentTree::InternalQueryInterval(const cTraversalData &TraversalData, cAddedExp to_add)
{
	auto &item=mItems[TraversalData.i];
	if(TraversalData.left_i>=TraversalData.left&&TraversalData.right_i<=TraversalData.right)
	{
		return (item.sum+item.CalculateAddedEffect(to_add))%mod;
	}
	int middle=(TraversalData.left_i+TraversalData.right_i)/2;
	ll result=0;
	if(middle>=TraversalData.left)
		result+=InternalQueryInterval(TraversalData.Left(), to_add+item.added_to_entire);
	if(middle<TraversalData.right)
		result+=InternalQueryInterval(TraversalData.Right(), to_add+item.added_to_entire);
	return result%mod;
}

ll cSegmentTree::QueryInterval(int left, int right) // both inclusive
{
	return InternalQueryInterval(CreateTraversalData(left, right), {});
}


///                   A D D


void cSegmentTree::InternalAddToRange(const cTraversalData &TraversalData, cAddedExp to_add)
{
	auto &item=mItems[TraversalData.i];
	if(TraversalData.left_i>=TraversalData.left&&TraversalData.right_i<=TraversalData.right)
	{
		item.added_to_entire+=to_add;
		item.sum+=item.CalculateAddedEffect(to_add);
		item.sum%=mod;
		return;
	}
	int middle=(TraversalData.left_i+TraversalData.right_i)/2;
	if(middle>=TraversalData.left)
		InternalAddToRange(TraversalData.Left(), to_add);
	if(middle<TraversalData.right)
		InternalAddToRange(TraversalData.Right(), to_add);
	MergerFunction(mItems[TraversalData.i], mItems[GetLeftChildIndex(TraversalData.i)], mItems[GetRightChildIndex(TraversalData.i)]);
}

void cSegmentTree::AddToRange(int left, int right, cAddedExp to_add)
{
	if(right==mSize-1)
	{
		right=mBottomRowSize-1;
	}
	InternalAddToRange(CreateTraversalData(left, right), to_add);
}
