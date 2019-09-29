ull isolate_highest_bit(ull x)
{
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    return x & ~(x >> 1);
}

ull ones_up_till_highest_bit(ull x) //   for example 0000'0000'0010'1101 -->  0000'0000'0011'1111
{
    return x == 0 ? 0 : (((isolate_highest_bit(x) - 1ull) << 1ull) | 1ull);
}


struct cSegmentData
{
    ll sum = 0;
    ll lazy_add_to_each = 0;
/////////////////////////////
    ll first_index, length = 1;
};

class cSegmentTree
{
    std::vector<cSegmentData> mItems; // the root has index 1
    static size_t GetParentIndex(size_t i) { return i / 2; }
    static size_t GetLeftChildIndex(size_t i) { return i * 2; }
    static size_t GetRightChildIndex(size_t i) { return GetLeftChildIndex(i) + 1; }
    int mSize = 0;
    int mBottomRowSize = 0;
    size_t mFirstSingleItemIndex = 0;
    struct cTraversalData
    {
        size_t i;
        int left_i, right_i;
        int left, right;
        cTraversalData(size_t i, int left_i, int right_i, int left, int right) : i(i), left_i(left_i), right_i(right_i), left(max(left_i, left)), right(min(right_i, right))
        {
        }
        cTraversalData Left() const
        {
            return cTraversalData(GetLeftChildIndex(i), left_i, (left_i + right_i) / 2, left, right);
        }
        cTraversalData Right() const
        {
            return cTraversalData(GetRightChildIndex(i), (left_i + right_i) / 2 + 1, right_i, left, right);
        }
    };
    cTraversalData CreateTraversalData(int left, int right) const { return cTraversalData(1, 0, mBottomRowSize - 1, left, right); }
    ll InternalQueryInterval(const cTraversalData &TraversalData, ll added_to_each);
    void InternalRangeAdd(const cTraversalData &TraversalData, ll to_add);
public:
    void Init(int Size);

    void RangeAdd(int left, int right, ll to_add);

    ll QueryInterval(int left, int right);

    void MergerFunction(cSegmentData &result, const cSegmentData &sub_a, const cSegmentData &sub_b)
    {
        result.sum = 
            sub_a.sum + sub_b.sum + 
            sub_a.lazy_add_to_each*sub_a.length + sub_b.lazy_add_to_each*sub_b.length;
    }
};

void cSegmentTree::Init(int Size)
{
    mSize = Size;
    int SizeRequired = Size == 1 ? 2 : 2 * ((int)ones_up_till_highest_bit(Size - 1) + 1);
    mItems.resize(SizeRequired);
    mFirstSingleItemIndex = SizeRequired / 2;
    mBottomRowSize = SizeRequired / 2;


    // 	InitItemsFunction(mItems.data()+mFirstSingleItemIndex, Size);
    for (int i = (int)mFirstSingleItemIndex; i < mFirstSingleItemIndex + Size; ++i)
    {
        mItems[i].sum = 0;
    }


    for (int i = (int)mFirstSingleItemIndex; i < SizeRequired; ++i)
    {
        mItems[i].first_index = i - mFirstSingleItemIndex;
    }
    for (int i = (int)mFirstSingleItemIndex + Size; i < SizeRequired; ++i)
    {
        mItems[i] = mItems[mFirstSingleItemIndex + Size - 1];
    }
    for (size_t i = mFirstSingleItemIndex - 1; i > 0; --i)
    {
        mItems[i].length = mItems[GetLeftChildIndex(i)].length * 2;
        mItems[i].first_index = mItems[GetLeftChildIndex(i)].first_index;
    }
    for (size_t i = mFirstSingleItemIndex - 1; i > 0; --i)
    {
        MergerFunction(mItems[i], mItems[GetLeftChildIndex(i)], mItems[GetRightChildIndex(i)]);
    }
}

ll cSegmentTree::InternalQueryInterval(const cTraversalData &TraversalData, ll added_to_each)
{
    auto &item = mItems[TraversalData.i];
    if (TraversalData.left_i >= TraversalData.left&&TraversalData.right_i <= TraversalData.right)
    {
        return item.sum + (item.lazy_add_to_each + added_to_each) * item.length;
    }
    int middle = (TraversalData.left_i + TraversalData.right_i) / 2;
    ll result = 0;
    if (middle >= TraversalData.left)
        result += InternalQueryInterval(TraversalData.Left(), added_to_each + item.lazy_add_to_each);
    if (middle < TraversalData.right)
        result += InternalQueryInterval(TraversalData.Right(), added_to_each + item.lazy_add_to_each);
    return result;
}

ll cSegmentTree::QueryInterval(int left, int right) // both inclusive
{
    return InternalQueryInterval(CreateTraversalData(left, right), 0);
}


///                   Increase


void cSegmentTree::InternalRangeAdd(const cTraversalData &TraversalData, ll to_add)
{
    auto &item = mItems[TraversalData.i];
    if (TraversalData.left_i >= TraversalData.left&&TraversalData.right_i <= TraversalData.right)
    {
        item.lazy_add_to_each += to_add;
        return;
    }
    int middle = (TraversalData.left_i + TraversalData.right_i) / 2;
    if (middle >= TraversalData.left)
        InternalRangeAdd(TraversalData.Left(), to_add);
    if (middle < TraversalData.right)
        InternalRangeAdd(TraversalData.Right(), to_add);
    MergerFunction(mItems[TraversalData.i], mItems[GetLeftChildIndex(TraversalData.i)], mItems[GetRightChildIndex(TraversalData.i)]);
}

void cSegmentTree::RangeAdd(int left, int right, ll to_add)
{
    InternalRangeAdd(CreateTraversalData(left, right), to_add);
}

void testSegmentTree()
{
    cSegmentTree st;
    st.Init(100);
    vector<ll> ref(100, 0);

    for (int c=0;;++c)
    {
        int l = rand() %100, r = rand() % 100;
        if (l > r)
            swap(l, r);
        if (c % 2)
        {
            int x = rand() % 3;
            st.RangeAdd(l, r, x);
            for (int i = l; i <= r; ++i)
                ref[i] += x;
        }
        else
        {
            ll s = st.QueryInterval(l, r);
            ll rs = 0;
            for (int i = l; i <= r; ++i)
                rs += ref[i];
            ASSERT(s == rs);
        }
    }
}