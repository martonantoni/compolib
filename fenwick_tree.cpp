/// ----------------------------------------------------------------------------------------------------------- fenwick_tree.cpp

template<class T> auto isolate_lowest_bit(T x)
{
	return x&(0-x);
}

class cFenwickTree
{
	std::vector<ll> a;
	size_t mSize=0;
public:
	cFenwickTree() {}
	cFenwickTree(int Size)
	{
		InitWithZero(Size);
	}
	void InitWithZero(int Size)
	{
		mSize=Size;
		a.resize(mSize+1, 0ll);
	}
	void Update(int Index, ll delta);
	ll GetSumUntil(int n) const; // sum of first n number
	ll GetSum_BetweenIndexes(int From, int To) const; // inclusive
};

void cFenwickTree::Update(int Index, ll delta)
{
	++Index;
	int i=(int)Index;
	while(i<=mSize)
	{
		a[i]+=delta;
		i+=isolate_lowest_bit(i);
	}
}

ll cFenwickTree::GetSumUntil(int n) const
{
	int i=n;
	ll sum=0;
	while(i>0)
	{
		sum+=a[i];
		i=i&~isolate_lowest_bit(i);
	}
	return sum;
}

ll cFenwickTree::GetSum_BetweenIndexes(int From, int To) const // inclusive
{
	return GetSumUntil(To+1)-GetSumUntil(From);
}
/// ----------------------------------------------------------------------------------------------------------- end of fenwick_tree.cpp
