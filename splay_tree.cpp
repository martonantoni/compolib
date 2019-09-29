template<class K, class V>
class tSplayTree // based on the lecture https://www.youtube.com/watch?v=G5QIXywcJlY
{                // for checking use online demo: https://www.cs.usfca.edu/~galles/visualization/SplayTree.html
	struct cNode
	{
		cNode *mParent=nullptr, *mLeft=nullptr, *mRight=nullptr;
		K mKey;
		V mValue;
		cNode(K Key): mKey(Key) {}
	};
	cNode *mRoot=nullptr;
	size_t mSize=0;
	void RotateThroughParent(cNode *Node, cNode *Y);
	void SplayNodeToRoot(cNode *Node);
	pair<cNode *, bool> FindNode(const K &Key); // does not splay, returns { last valid, false } if key not found, { node with key, true } if found
	void DeleteRoot();
public:
	V &operator[](const K &Key);
	void Insert(const K &Key, const V &Value) { (*this)[Key]=Value; }
	V *Find(const K &Key); // returns nullptr if not found
	void Delete(const K &Key);
	size_t size() const { return mSize; }
};

template<class K, class V> std::pair<typename tSplayTree<K, V>::cNode *, bool> tSplayTree<K, V>::FindNode(const K &Key)
{
	if(!mRoot)
		return std::make_pair(mRoot, false);
	for(cNode *Node=mRoot;;)
	{
		if(Key<Node->mKey)
		{
			if(!Node->mLeft)
				return std::make_pair(Node, false);
			Node=Node->mLeft;
		}
		else if(Node->mKey<Key)
		{
			if(!Node->mRight)
				return std::make_pair(Node, false);
			Node=Node->mRight;
		}
		else
		{
			return std::make_pair(Node, true);
		}
	}
}

template<class K, class V> void tSplayTree<K, V>::RotateThroughParent(cNode *Node, cNode *Y)
{
	if(Y->mLeft==Node) // is left child?
	{
		if(Y==mRoot)
		{
			mRoot=Node;
			Node->mParent=nullptr;
		}
		else
		{
			(Y->mParent->mLeft==Y?Y->mParent->mLeft:Y->mParent->mRight)=Node;
			Node->mParent=Y->mParent;
		}
		cNode *B=Node->mRight;
		Node->mRight=Y; Y->mParent=Node;
		Y->mLeft=B;
		if(B)
			B->mParent=Y;
	}
	else // right child:
	{
		ASSERT(Node->mParent->mRight==Node);
		if(Y==mRoot)
		{
			mRoot=Node;
			Node->mParent=nullptr;
		}
		else
		{
			(Y->mParent->mLeft==Y?Y->mParent->mLeft:Y->mParent->mRight)=Node;
			Node->mParent=Y->mParent;
		}
		cNode *B=Node->mLeft;
		Node->mLeft=Y; Y->mParent=Node;
		Y->mRight=B;
		if(B)
			B->mParent=Y;
	}
}

template<class K, class V> void tSplayTree<K, V>::SplayNodeToRoot(cNode *Node)
{
	while(mRoot!=Node)
	{
		cNode *P=Node->mParent;
		if(P==mRoot)
		{
			// case 3:
			RotateThroughParent(Node, mRoot);
			ASSERT(mRoot==Node);
			return;
		}
		else
		{
			cNode *G=P->mParent;
			if((P==G->mLeft&&Node==P->mRight)||(P==G->mRight&&Node==P->mLeft))
			{
				// case 1, the "zig-zag"
				RotateThroughParent(Node, P);
				RotateThroughParent(Node, G);
				ASSERT(G->mParent==Node&&P->mParent==Node);
			}
			else
			{
				// case 2, the "zig-zig"
				RotateThroughParent(P, G);
				RotateThroughParent(Node, P);
				ASSERT(P->mParent==Node&&G->mParent==P);
			}
		}
	}
}

template<class K, class V> V &tSplayTree<K, V>::operator[](const K &Key)
{
	if(!mRoot)
	{
		mRoot=new cNode(Key);
		++mSize;
		return mRoot->mValue;
	}
	cNode *Node; bool Found;
	tie(Node, Found)=FindNode(Key);
	if(Found)
	{
		SplayNodeToRoot(Node);
		return mRoot->mValue;
	}
	cNode *NewNode=new cNode(Key);
	++mSize;
	NewNode->mParent=Node;
	if(Key<Node->mKey)
	{
		ASSERT(!Node->mLeft);
		Node->mLeft=NewNode;
	}
	else
	{
		ASSERT(!Node->mRight);
		Node->mRight=NewNode;
	}
	SplayNodeToRoot(NewNode);
	return NewNode->mValue;
}

template<class K, class V> V *tSplayTree<K, V>::Find(const K &Key)
{
	if(!mRoot)
		return nullptr;
	cNode *Node; bool Found;
	tie(Node, Found)=FindNode(Key);
	SplayNodeToRoot(Node);
	return Found?&Node->mValue:nullptr;
}

template<class K, class V> void tSplayTree<K, V>::DeleteRoot()
{
	ASSERT(mRoot);
	--mSize;
	cNode *OldRoot=mRoot;
	if(!OldRoot->mLeft||!OldRoot->mRight) // root had only one child
	{
		mRoot=OldRoot->mRight?OldRoot->mRight:OldRoot->mLeft;
		if(mRoot)
			mRoot->mParent=nullptr;
		delete OldRoot;
		return;
	}
	// two children:
	cNode *RightSubTreeRoot=OldRoot->mRight;
	mRoot=OldRoot->mLeft; 
	mRoot->mParent=nullptr;
	cNode *RightMost=mRoot;
	while(RightMost->mRight)
		RightMost=RightMost->mRight;
	SplayNodeToRoot(RightMost);
	ASSERT(!mRoot->mRight);
	mRoot->mRight=RightSubTreeRoot; RightSubTreeRoot->mParent=mRoot;
	delete OldRoot;
}

template<class K, class V> void tSplayTree<K, V>::Delete(const K &Key)
{
	if(!mRoot)
		return;
	cNode *Node; bool Found;
	tie(Node, Found)=FindNode(Key);
	SplayNodeToRoot(Node);
	if(!Found)
		return;
	DeleteRoot();
}

#include <random>

void TestSplayTree()
{
	tSplayTree<string, int> splay_tree;
	map<string, int> reference_tree;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> value_gen(-1'000'000, 1'000'000);
	std::uniform_int_distribution<int> key_gen('a', 'c');

	ll sum=0;
	string key;
	key.resize(15);
	{
		cLogPerformance_Guard PerfGuard("test_splay_tree");
		for(int c=0; c<1'000'000; ++c)
		{
			int value=value_gen(gen);
			if(value%5>0)
			{
				for(int i=0; i<14; ++i)
					key[i]='a';
				key[14]=key_gen(gen);
			}
			else
			{
				for(int i=0; i<15; ++i)
					key[i]=key_gen(gen);
			}
// 			if(key%10>0)
// 				key=500+key%50;
			if(c>10'000&&0==c%10)
			{
	//			splay_tree.Delete(key);
			//	reference_tree.erase(key);
			}
			else
			{
	//			int *v=splay_tree.Find(key);
	//			auto i=reference_tree.find(key);
		//		ASSERT((i!=reference_tree.end()&&v&&i->second==*v)||(i==reference_tree.end()&&!v));
	//			sum+=splay_tree[key];
		//		splay_tree[key]=value;
		//		sum+=reference_tree[key];
				reference_tree[key]=value;
			}
		}
	}
	printf("%lld", sum);
}
