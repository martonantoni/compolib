int inversions=0;

template<class K, class V>
class tTreap
{
	std::mt19937 mRandomGenerator;
	std::uniform_int_distribution<int> mPriorityGenerator;
	struct cNode
	{
		K mKey;
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
	std::pair<cNode *, cNode *> Split(cNode *Root, const K &Key);
	cNode *Merge(cNode *Left, cNode *Right);
	cNode *Insert(cNode *NewNode, cNode *Root);
	cNode *Erase(const K &Key, cNode *Root);
	static int GetSize(cNode *Root) { return Root?Root->mSize:0; }
public:
	tTreap(): mRandomGenerator(42424242), mPriorityGenerator(0, std::numeric_limits<int>::max()) {}
	void Insert(const K &Key, const V &Value);
	void Erase(const K &Key);
	V *Find(const K &Key);
	size_t size() const { return mRoot?mRoot->mSize:0; }
};

template<class K, class V>
V *tTreap<K, V>::Find(const K &Key)
{
	cNode *i=mRoot;
	while(i)
	{
		if(Key<i->mKey)
			i=i->mLeft;
		else if(i->mKey<Key)
			i=i->mRight;
		else
			return &i->mValue;
	}
	return nullptr;
}

template<class K, class V>
auto tTreap<K, V>::Split(cNode *Root, const K &Key)->std::pair<cNode *, cNode *>
{
	if(!Root)
		return make_pair(nullptr, nullptr);
	if(Key>Root->mKey)
	{
		auto SplitRight=Split(Root->mRight, Key);
		Root->mRight=SplitRight.first;
		Root->FixSize();
		return make_pair(Root, SplitRight.second);
	}
	auto SplitLeft=Split(Root->mLeft, Key);
	Root->mLeft=SplitLeft.second;
	Root->FixSize();
	return make_pair(SplitLeft.first, Root);
}

template<class K, class V>
auto tTreap<K, V>::Merge(cNode *Left, cNode *Right)->cNode *
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

template<class K, class V>
void tTreap<K, V>::Insert(const K &Key, const V &Value)
{
	cNode *NewNode=new cNode;
	NewNode->mKey=Key;
	NewNode->mValue=Value;
	NewNode->mPriority=mPriorityGenerator(mRandomGenerator);
	mRoot=Insert(NewNode, mRoot);
}

template<class K, class V>
auto tTreap<K, V>::Insert(cNode *NewNode, cNode *Root)->cNode *
{
	if(!Root)
		return NewNode;
	if(NewNode->mPriority>Root->mPriority)
	{
		// NewNode must be above Root to maintain heap property
		tie(NewNode->mLeft, NewNode->mRight)=Split(Root, NewNode->mKey);
		inversions+=GetSize(NewNode->mRight);
		NewNode->FixSize();
		return NewNode;
	}
	if(NewNode->mKey<Root->mKey)
	{
		inversions+=GetSize(Root->mRight)+1;
		Root->mLeft=Insert(NewNode, Root->mLeft);
	}
	else
		Root->mRight=Insert(NewNode, Root->mRight);
	Root->FixSize();
	return Root;
}

template<class K, class V>
void tTreap<K, V>::Erase(const K &Key)
{
	mRoot=Erase(Key, mRoot);
}

template<class K, class V>
auto tTreap<K, V>::Erase(const K &Key, cNode *Root)->cNode *
{
	if(!Root)
		return Root;
	if(Key<Root->mKey)
	{
		Root->mLeft=Erase(Key, Root->mLeft);
		Root->FixSize();
		return Root;
	}
	else if(Root->mKey<Key)
	{
		Root->mRight=Erase(Key, Root->mRight);
		Root->FixSize();
		return Root;
	}
	else
	{
		auto NewRoot=Merge(Root->mLeft, Root->mRight);
		delete Root;
		return NewRoot;
	}
}



void Solve()
{
	RI(n);
	inversions=0;
	tTreap<int, int> tree;
	for(int i=1; i<=n; ++i)
	{
		RI(x);
		tree.Insert(x, x);
	}
	//   printf("inversions: %d\n", inversions);
	if(inversions%2)
		printf("NO\n");
	else
		printf("YES\n");
}
