std::mt19937 TreapRandomGenerator(42424242);
std::uniform_int_distribution<int> TreapPriorityGenerator(0, std::numeric_limits<int>::max());

namespace Treap
{
	struct cNode
	{
		using cKey=int; /// <--- change key's type here
		cKey mKey;
		// add augmentation / payload here:
		cInterval mInterval;
		int mMaxEndpoint;
		void MaintainAugmentation()
		{
			mMaxEndpoint=max(mInterval.high,
				max(mLeft?mLeft->mMaxEndpoint:numeric_limits<decltype(cInterval::high)>::min(),
					mRight?mRight->mMaxEndpoint:numeric_limits<decltype(cInterval::high)>::min()));
		}
		// construction:
		cNode(const cInterval &Interval): mInterval(Interval), mKey(Interval.low), mMaxEndpoint(Interval.high) {}
		cNode(int low, int high): mInterval(low, high), mKey(low), mMaxEndpoint(high) {}

		// do not touch:
		int mPriority=TreapPriorityGenerator(TreapRandomGenerator);
		cNode *mLeft=nullptr, *mRight=nullptr;
		int mSize=1;
		void FixSize()
		{
			mSize=(mLeft?mLeft->mSize:0)+(mRight?mRight->mSize:0)+1;
			MaintainAugmentation();
		}
	};
	bool CompareKeys(const cNode::cKey &a, const cNode::cKey &b)
	{
		return a<b;
	}

	std::pair<cNode *, cNode *> Split(cNode *Root, const cNode::cKey &Key);
	cNode *Merge(cNode *Left, cNode *Right);
	cNode *Insert(cNode *Root, cNode *NewNode);
	cNode *Erase(cNode *Root, const cNode::cKey &Key); // deletes the Node with Key if found
	cNode *Remove(cNode *Root, cNode *NodeToRemove); // does not delete NodeToRemove
	cNode *Find(cNode *Root, const cNode::cKey &Key);
	cNode *GetNTh(cNode *Root, int n);
	size_t size(cNode *Root) { return Root?Root->mSize:0; }
	cNode *FindIntersectingInterval(cNode *root, int low, int high);

	cNode *Find(cNode *Root, const cNode::cKey &Key)
	{
		cNode *i=Root;
		while(i)
		{
			if(CompareKeys(Key, i->mKey))
				i=i->mLeft;
			else if(CompareKeys(i->mKey, Key))
				i=i->mRight;
			else
				return i;
		}
		return nullptr;
	}

	cNode *GetNTh(cNode *Root, int n)
	{
		if(!Root)
			return nullptr;
		int Left=(int)size(Root->mLeft), Right=(int)size(Root->mRight);
		if(n<Left)
			return GetNTh(Root->mLeft, n);
		else if(n==Left+1)
			return Root;
		else
			return GetNTh(Root->mRight, n-(Left+1));
	}

	std::pair<cNode *, cNode *> Split(cNode *Root, const cNode::cKey &Key)
	{
		if(!Root)
			return make_pair(nullptr, nullptr);
		if(CompareKeys(Key, Root->mKey))
		{
			auto SplitLeft=Split(Root->mLeft, Key);
			Root->mLeft=SplitLeft.second;
			Root->FixSize();
			return make_pair(SplitLeft.first, Root);
		}
		else
		{
			auto SplitRight=Split(Root->mRight, Key);
			Root->mRight=SplitRight.first;
			Root->FixSize();
			return make_pair(Root, SplitRight.second);
		}
	}

	cNode *Merge(cNode *Left, cNode *Right)
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

	cNode *Insert(cNode *Root, cNode *NewNode)
	{
		if(!Root)
		{
			NewNode->mLeft=NewNode->mRight=nullptr;
			return NewNode;
		}
		if(NewNode->mPriority>Root->mPriority)
		{
			// NewNode must be above Root to maintain heap property
			tie(NewNode->mLeft, NewNode->mRight)=Split(Root, NewNode->mKey);
			NewNode->FixSize();
			return NewNode;
		}
		if(CompareKeys(NewNode->mKey, Root->mKey))
			Root->mLeft=Insert(Root->mLeft, NewNode);
		else
			Root->mRight=Insert(Root->mRight, NewNode);
		Root->FixSize();
		return Root;
	}

	cNode *Erase(cNode *Root, const cNode::cKey &Key)
	{
		if(!Root)
			return Root;
		if(CompareKeys(Key, Root->mKey))
		{
			Root->mLeft=Erase(Root->mLeft, Key);
			return Root;
		}
		else if(CompareKeys(Root->mKey, Key))
		{
			Root->mRight=Erase(Root->mRight, Key);
			return Root;
		}
		else
		{
			auto NewRoot=Merge(Root->mLeft, Root->mRight);
			NewRoot->FixSize();
			delete Root;
			return NewRoot;
		}
	}

	cNode *Remove(cNode *Root, cNode *NodeToRemove)
	{
		if(!Root)
			return Root;
		if(Root==NodeToRemove)
		{
			auto NewRoot=Merge(Root->mLeft, Root->mRight);
			NewRoot->FixSize();
			return NewRoot;
		}
		if(CompareKeys(NodeToRemove->mKey, Root->mKey))
		{
			Root->mLeft=Remove(Root->mLeft, NodeToRemove);
			return Root;
		}
		else if(CompareKeys(Root->mKey, NodeToRemove->mKey))
		{
			Root->mRight=Remove(Root->mRight, NodeToRemove);
			return Root;
		}
		else
			return nullptr; // shouldn't happen
	}
}

using namespace Treap;

void Solve()
{
	RI(n);
	vector<cNode> nodes(n);
	cNode *treap=nullptr;
	for(auto &node: nodes)
	{
		scanf("%d", &node.mKey);
		treap=Insert(treap, &node);
	}
	RI(q);
	for(int i=0; i<q; ++i)
	{
		RI(type);
		if(type==1)
		{
			RII(x, k);
			treap=Remove(treap, &nodes[x]);
			nodes[x].mKey=k;
			treap=Insert(&nodes[x]);
		}
		else
		{

		}
	}
}