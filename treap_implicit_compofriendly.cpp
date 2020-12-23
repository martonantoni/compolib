std::mt19937 TreapRandomGenerator(42424242);
std::uniform_int_distribution<int> TreapPriorityGenerator(0, std::numeric_limits<int>::max());

namespace ImplicitTreap
{

    struct cNode
    {
        // payload:
        int mValue;

        // do not touch:
        int mPriority;
        cNode* mLeft = nullptr;
        cNode* mRight = nullptr;
        cNode* mParent = nullptr;
        int mSize = 1;
        void FixSize()
        {
            mSize = (mLeft ? mLeft->mSize : 0) + (mRight ? mRight->mSize : 0) + 1;
        }
    };
    pair<cNode*, cNode*> InternalSplit(cNode* Root, int split_pos, int passed_left_size);
    cNode* Merge(cNode* Left, cNode* Right);

    void InOrderPrint(cNode* Root)
    {
        if (!Root) return;
        InOrderPrint(Root->mLeft);
        printf("%d ", Root->mValue);
        InOrderPrint(Root->mRight);
    }

    int GetSize(cNode* Root) { return Root ? Root->mSize : 0; }
    cNode* Front(cNode* Root)
    {
        return Root->mLeft ? Front(Root->mLeft) : Root;
    }
    cNode* Back(cNode* Root)
    {
        return Root->mRight ? Front(Root->mRight) : Root;
    }

    void Init(cNode* Node);
    pair<cNode*, cNode*> Split(cNode* Root, int SplitPosition); // destroys this treap, returns new roots

    pair<cNode*, cNode*> InternalSplit(cNode* Root, int split_pos, int passed_left_size)
    {
        if (!Root)
            return make_pair(nullptr, nullptr);
        int RootKey = GetSize(Root->mLeft) + passed_left_size + 1;
        if (split_pos >= RootKey)
        {
            auto SplitRight = InternalSplit(Root->mRight, split_pos, RootKey);

            if (Root->mRight && Root->mRight->mParent == Root)
                Root->mRight->mParent = nullptr;
            Root->mRight = SplitRight.first;
            if(Root->mRight)
                Root->mRight->mParent = Root;

            Root->FixSize();
            return make_pair(Root, SplitRight.second);
        }
        auto SplitLeft = InternalSplit(Root->mLeft, split_pos, passed_left_size);

        if (Root->mLeft && Root->mLeft->mParent == Root)
            Root->mLeft->mParent = nullptr;
        Root->mLeft = SplitLeft.second;
        if (Root->mLeft)
            Root->mLeft->mParent = Root;

        Root->FixSize();
        return make_pair(SplitLeft.first, Root);
    }

    cNode* Merge(cNode* Left, cNode* Right)
    {
        if (!Left || !Right)
            return Left ? Left : Right;
        if (Left->mPriority > Right->mPriority)
        {   // higher priority will be nearer the root
            // so in this case Left will be above Right
            Left->mRight = Merge(Left->mRight, Right);
            Left->mRight->mParent = Left;
            Left->FixSize();
            return Left;
        }
        else
        {
            // in this case Rigth will be above Left
            Right->mLeft = Merge(Left, Right->mLeft);
            Right->mLeft->mParent = Right;
            Right->FixSize();
            return Right;
        }
    }

    void Init(cNode* NewNode)
    {
        NewNode->mPriority = TreapPriorityGenerator(TreapRandomGenerator);
    }

    pair<cNode*, cNode*> Split(cNode* Root, int SplitPosition)
    {
        return InternalSplit(Root, SplitPosition, 0);
    }


    int GetIndex(cNode* root, cNode* node)
    {
        int index = node->mLeft ? node->mLeft->mSize : 0;
        while (node != root)
        {
            if (node == node->mParent->mRight)
            {
                index += node->mParent->mSize - node->mSize;
            }
            node = node->mParent;
        }
        return index;
    }

}
