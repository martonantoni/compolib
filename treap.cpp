std::mt19937 TreapRandomGenerator(42424242);
std::uniform_int_distribution<int> TreapPriorityGenerator(0, std::numeric_limits<int>::max());


template<class K, class V>
class tTreap
{
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
public:
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
	if(Key>=Root->mKey)
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
	NewNode->mPriority=TreapPriorityGenerator(TreapRandomGenerator);
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
		NewNode->FixSize();
		return NewNode;
	}
	if(NewNode->mKey<Root->mKey)
		Root->mLeft=Insert(NewNode, Root->mLeft);
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



#include <random>

void TestTreap()
{
	tTreap<int, int> treap;
	map<int, int> reference_tree;

//	std::random_device rd;
	std::mt19937 gen(33);
	std::uniform_int_distribution<int> value_gen(-1'000'000, 1'000'000);
//	std::uniform_int_distribution<int> key_gen('a', 'c');
	std::uniform_int_distribution<int> key_gen(0,100);

	ll sum=0;
// 	string key;
// 	key.resize(15);
	{
		cLogPerformance_Guard PerfGuard("test_treap");
		for(int c=0; c<10'000'000; ++c)
		{
			int value=value_gen(gen);
			int key=key_gen(gen);
// 			for(int i=0; i<15; ++i)
// 				key[i]=key_gen(gen);
			if(c>10'000&&0==c%10)
			{
		//		treap.Erase(key);
				reference_tree.erase(key);
			}
			else
			{
//				int *v=treap.Find(key);
// 				auto i=reference_tree.find(key);
// 				ASSERT((i!=reference_tree.end()&&v&&i->second==*v)||(i==reference_tree.end()&&!v));
 				reference_tree[key]=value;

// 				if(!v)
// 					treap.Insert(key, value);
// 				else
// 					*v=value;

	//			sum+=splay_tree[key];
		//		splay_tree[key]=value;
		//		sum+=reference_tree[key];
	//			reference_tree[key]=value;

			}
		}
	}
//	printf("%lld", sum);
}
