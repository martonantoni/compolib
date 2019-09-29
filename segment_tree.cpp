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
	template<class F> void InternalForInterval(const cTraversalData &TraversalData, F Func);
	template<class F> void InternalUpdate(size_t i, int left_i, int right_i, int i_to_update, F Func);
public:
	template<class IF> void Init(int Size, IF InitItemsFunction);
	// InitItemsFunction's signature:
	// [](T *Items, int Size)

	template<class F> void Update(int i, F UpdateFunc);
	template<class F> void ForInterval(int left, int right, F Func);
	// left & right are both inclusive

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
void tSegmentTree<T, P>::InternalForInterval(const cTraversalData &TraversalData, F Func)
{
	auto &item=mItems[TraversalData.i];
	if(TraversalData.left_i>=TraversalData.left&&TraversalData.right_i<=TraversalData.right)
	{
		Func(item);
		return;
	}
	int middle=(TraversalData.left_i+TraversalData.right_i)/2;
	if(middle>=TraversalData.left)
		InternalForInterval(TraversalData.Left(), Func);
	if(middle<TraversalData.right)
		InternalForInterval(TraversalData.Right(), Func);
}

template<class T, class P>
template<class F>
void tSegmentTree<T, P>::ForInterval(int left, int right, F Func) // both inclusive
{
	InternalForInterval(CreateTraversalData(left, right), Func);
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
	int x;
};

class cSegmentTreeBase
{
protected:
	void MergerFunction(cSegmentData &result, const cSegmentData &sub_a, const cSegmentData &sub_b)
	{
		result.x=max(sub_a.x, sub_b.x);
	}
};
