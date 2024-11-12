struct cHeapElement
{
    int freq;
    int name_idx;
};


class cHeap // strongest at top
{
    static bool IsStronger(const cHeapElement& a, const cHeapElement& b) { return a.freq < b.freq || (a.freq == b.freq && a.name_idx < b.name_idx); }
    std::vector<cHeapElement> mHeap;
    int mContainerSize = 1;
    static consteval int root() { return 1; }
    static int leftChild(int index) { return 2 * index; }
    static int rightChild(int index) { return 2 * index + 1; }
    static int parent(int index) { return index / 2; }
    unordered_map<int, int> mIndex; // name_idx -> heap_idx
    void swap(int a, int b) 
    { 
        mIndex[mHeap[a].name_idx] = b;
        mIndex[mHeap[b].name_idx] = a;
        std::swap(mHeap[a], mHeap[b]); 
    }
public:
    cHeap() { mHeap.push_back(cHeapElement{}); } // dummy element at index 0
    const cHeapElement& top() const { return mHeap[root()]; }
    void push(int name_idx, int freq);
 //   void pop();
    size_t size() const { return mContainerSize - 1; }
    bool empty() const { return size() == 0; }

    void increase(int name_idx, int freq)
    {
        int heap_idx = mIndex[name_idx];
        if(heap_idx==0)
        {
            push(name_idx, freq);
            return;
        }
        mHeap[heap_idx].freq += freq;

// goes toward bottom:
        for (;;)
        {
            int left = leftChild(heap_idx);
            int right = rightChild(heap_idx);
            if (left >= mContainerSize)
                break;
            if (right >= mContainerSize)
            {
                if (IsStronger(mHeap[left], mHeap[heap_idx]))
                    swap(left, heap_idx);
                break;
            }
            if (IsStronger(mHeap[left], mHeap[heap_idx]))
            {
                if (IsStronger(mHeap[left], mHeap[right]))
                {
                    swap(left, heap_idx);
                    heap_idx = left;
                }
                else
                {
                    swap(right, heap_idx);
                    heap_idx = right;
                }
            }
            else if (IsStronger(mHeap[right], mHeap[heap_idx]))
            {
                swap(right, heap_idx);
                heap_idx = right;
            }
            else
                break;
        }
    }

    void merge(const cHeap& other)
    {
        for(auto& item : other.mHeap | views::drop(1))
        {
            increase(item.name_idx, item.freq);
        }    
    }
};

void cHeap::push(int name_idx, int freq)
{
    cHeapElement value{ freq, name_idx };
    mHeap.push_back(value);
    mIndex[value.name_idx] = mContainerSize;
    ++mContainerSize;
    int index = mContainerSize - 1;
    while (index != root() && IsStronger(mHeap[index], mHeap[parent(index)]))
    {
//        std::swap(mHeap[index], mHeap[parent(index)]);
        swap(index, parent(index));
        index = parent(index);
    }
}
