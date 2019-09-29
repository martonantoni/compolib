#include <unordered_map>
#include <memory>
#include <tuple>

using ull=unsigned long long;
using ll=long long;
static const ll mod=1'000'000'007;

class cTrie
{
	struct cNode
	{
		std::unordered_map<char, std::unique_ptr<cNode>> mSubNodes;
// challenge specific:
		int mSubWords=0;
	};
	cNode mRoot;
public:
	void Push(const std::string &Word);
// challenge specific:
	int GetCount(const std::string &Word);
};

void cTrie::Push(const std::string &Word)
{
	cNode *Node=&mRoot;
	++mRoot.mSubWords;  // challenge specific
	for(char c: Word)
	{
		auto &NextNode=Node->mSubNodes[c];
		if(!NextNode)
			NextNode=std::make_unique<cNode>();
		Node=NextNode.get();
		++Node->mSubWords; // challenge specific
	}
}

int cTrie::GetCount(const std::string &Word)
{
	cNode *Node=&mRoot;
	for(char c: Word)
	{
		auto &NextNode=Node->mSubNodes[c];
		if(!NextNode)
		{
			return 0;
		}
		Node=NextNode.get();
	}
	return Node->mSubWords;
}

void Init()
{
}

cTrie trie;

void Solve()
{
	string op, word;
	cin >> op >> word;
	if(op[0]=='a')
	{
		trie.Push(word);
	}
	else
	{
		printf("%d\n", trie.GetCount(word));
	}
}

int main()
{
	Init();
	int t=1;
	scanf("%d", &t);
	for(int tc=1; tc<=t; ++tc)
	{
		Solve();
	}
}