std::mt19937 TreapRandomGenerator(42424242);
std::uniform_int_distribution<int> TreapPriorityGenerator(0, std::numeric_limits<int>::max());

template<class V>
class tImpicitTreap
{
	struct cNode
	{
		V mValue;
		int mPriority;
		cNode *mLeft=nullptr, *mRight=nullptr;
		int mSize=1;
		void FixSize()
		{
			mSize=(mLeft?mLeft->mSize:0)+(mRight?mRight->mSize:0)+1;
		}
	};
	cNode *mRoot=nullptr;
	static std::pair<cNode *, cNode *> Split(cNode *Root, int split_pos, int passed_left_size);
	static cNode *Merge(cNode *Left, cNode *Right);
	static void InOrderPrint(cNode *Root)
	{
		if(!Root) return;
		InOrderPrint(Root->mLeft);
		printf("%d ", Root->mValue);
		InOrderPrint(Root->mRight);
	}
	static int GetSize(const cNode *Root) { return Root?Root->mSize:0; }
	static V Front(const cNode *Root)
	{
		return Root->mLeft?Front(Root->mLeft):Root->mValue;
	}
	static V Back(const cNode *Root)
	{
		return Root->mRight?Front(Root->mRight):Root->mValue;
	}
	tImpicitTreap(cNode *Root): mRoot(Root) {}
public:
	tImpicitTreap()=default;
	tImpicitTreap(const tImpicitTreap<V> &)=delete;
	tImpicitTreap<V> &operator=(const tImpicitTreap<V> &)=delete;
	tImpicitTreap(tImpicitTreap &&Other): mRoot(Other.mRoot)
	{
		Other.mRoot=nullptr;
	}
	tImpicitTreap &operator=(tImpicitTreap<V> &&Other)
	{
		if(&Other!=this)
		{
			mRoot=Other.mRoot;
			Other.mRoot=nullptr;
		}
		return *this;
	}
	size_t size() const { return GetSize(mRoot); }
	static tImpicitTreap<V> Construct(const V &Value);
	static tImpicitTreap<V> Merge(tImpicitTreap<V> &&Left, tImpicitTreap<V> &&Right);
	pair<tImpicitTreap<V>, tImpicitTreap<V>> Split(int SplitPosition); // destroys this treap

	void InOrderPrint()
	{
		InOrderPrint(mRoot);
	}
	V Front() const { return Front(mRoot); }
	V Back() const { return Back(mRoot); }
};


template<class V>
auto tImpicitTreap<V>::Split(cNode *Root, int split_pos, int passed_left_size)->std::pair<cNode *, cNode *>
{
	if(!Root)
		return make_pair(nullptr, nullptr);
	int RootKey=GetSize(Root->mLeft)+passed_left_size+1;
	if(split_pos>=RootKey)
	{
		auto SplitRight=Split(Root->mRight, split_pos, RootKey);
		Root->mRight=SplitRight.first;
		Root->FixSize();
		return make_pair(Root, SplitRight.second);
	}
	auto SplitLeft=Split(Root->mLeft, split_pos, passed_left_size);
	Root->mLeft=SplitLeft.second;
	Root->FixSize();
	return make_pair(SplitLeft.first, Root);
}

template<class V>
auto tImpicitTreap<V>::Merge(cNode *Left, cNode *Right)->cNode *
{
	if(!Left||!Right)
		return Left?Left:Right;
	if(Left->mPriority>Right->mPriority)
	{   // higher priority will be nearer the root
		// so in this case Left will be above Right
		Left->mRight=Merge(Left->mRight, Right);
		Left->FixSize();
		return Left;
	}
	else
	{
		// in this case Rigth will be above Left
		Right->mLeft=Merge(Left, Right->mLeft);
		Right->FixSize();
		return Right;
	}
}

template<class V>
tImpicitTreap<V> tImpicitTreap<V>::Construct(const V &Value)
{
	tImpicitTreap<V> NewTreap;
	cNode *NewNode=new cNode;
	NewNode->mValue=Value;
	NewNode->mPriority=TreapPriorityGenerator(TreapRandomGenerator);
	NewTreap.mRoot=NewNode;
	return NewTreap;
}

template<class V>
tImpicitTreap<V> tImpicitTreap<V>::Merge(tImpicitTreap<V> &&Left, tImpicitTreap<V> &&Right)
{
	tImpicitTreap<V> NewTreap(Merge(Left.mRoot, Right.mRoot));
	Left.mRoot=Right.mRoot=nullptr;
	return NewTreap;
}

template<class V>
pair<tImpicitTreap<V>, tImpicitTreap<V>> tImpicitTreap<V>::Split(int SplitPosition)
{
	auto SplitResult=Split(mRoot, SplitPosition, 0);
	mRoot=nullptr;
	return make_pair(tImpicitTreap<V>(SplitResult.first), tImpicitTreap<V>(SplitResult.second));
}

void Solve()
{
	RII(n, m);
	RIV(a, n);
	tImpicitTreap<int> treap=tImpicitTreap<int>::Construct(a[0]);
	for(int i=1; i<n; ++i)
	{
		tImpicitTreap<int> right_treap=tImpicitTreap<int>::Construct(a[i]);
		treap=tImpicitTreap<int>::Merge(move(treap), move(right_treap));
	}
//	treap.InOrderPrint(); printf("\n");
	for(int i=0; i<m; ++i)
	{
		RIII(t, l, r);
		auto treap_a_bc=treap.Split(l-1);
// 		printf("treap_a: "); treap_a_bc.first.InOrderPrint();
// 		printf("\ntreap_bc: "); treap_a_bc.second.InOrderPrint();
// 		printf("\n");
		auto treap_b_c=treap_a_bc.second.Split(r-l+1);
// 		printf("treap_b: "); treap_b_c.first.InOrderPrint();
// 		printf("\ntreap_c: "); treap_b_c.second.InOrderPrint();
// 		printf("\n");
		if(t==1)
		{
			treap=tImpicitTreap<int>::Merge(move(treap_b_c.first), move(treap_a_bc.first));
			treap=tImpicitTreap<int>::Merge(move(treap), move(treap_b_c.second));
		}
		else
		{
			treap=tImpicitTreap<int>::Merge(move(treap_a_bc.first), move(treap_b_c.second));
			treap=tImpicitTreap<int>::Merge(move(treap), move(treap_b_c.first));
		}
//		treap.InOrderPrint(); printf("\n");
	}
	printf("%d %d\n", treap.Front(), treap.Back());
	treap.InOrderPrint();
}
