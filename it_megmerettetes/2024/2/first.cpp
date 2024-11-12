#include "aoc.h"
#include "bmp.h"
#include "utils.h"


struct cIntrusiveDisjointSetsItemBase
{
	size_t mParentIndex = -1;
	int mRank = 0;
	bool IsRoot() const { return mParentIndex == -1; }
};

template<class T> // T must have cIntrusiveDisjointSetsItemBase as parent class
class tIntrusiveDisjointSets
{
	std::vector<T> mItems;
	size_t mNumberOfRoots = 0;
	void JoinIntoSet(size_t JoiningSet, size_t JoinedSet)
	{
		mItems[JoiningSet].mParentIndex = JoinedSet;
		// challenge specific code for union:
		mItems[JoinedSet].SetJoined(mItems[JoiningSet]);
		--mNumberOfRoots;
	}
public:
	tIntrusiveDisjointSets() = default;
	tIntrusiveDisjointSets(tIntrusiveDisjointSets<T>& other) = delete;
	tIntrusiveDisjointSets& operator=(tIntrusiveDisjointSets<T>& other) = delete;
	tIntrusiveDisjointSets& operator=(tIntrusiveDisjointSets<T>&& other)
	{
		if (&other == this)
			return *this;
		mItems = std::move(other.mItems);
		mNumberOfRoots = other.mNumberOfRoots;
		other.mNumberOfRoots = 0;
		return *this;
	}
	void Init(size_t NumberOfItems)
	{
		mItems.resize(NumberOfItems);
		mNumberOfRoots = NumberOfItems;
	}
	void Init(std::vector<T>&& Items)
	{
		mItems = std::move(Items);
		mNumberOfRoots = mItems.size();
	}
	int AddItem()
	{
		mItems.emplace_back();
		++mNumberOfRoots;
		return (int)mItems.size() - 1;
	}
	template<class U> int AddItem(U&& item)
	{
		mItems.emplace_back(std::forward<U>(item));
		++mNumberOfRoots;
		return (int)mItems.size() - 1;
	}
	size_t GetRoot(size_t i) const
	{
		auto& Item = mItems[i];
		if (Item.mParentIndex == -1)
			return i;
		// path compression:
		auto RootIndex = GetRoot(Item.mParentIndex);
		const_cast<size_t&>(Item.mParentIndex) = RootIndex;
		return RootIndex;
	}
	void Union(size_t a, size_t b)
	{
		auto a_root_i = GetRoot(a), b_root_i = GetRoot(b);
		if (a_root_i == b_root_i)
			return;
		if (mItems[a_root_i].mRank > mItems[b_root_i].mRank)
		{
			JoinIntoSet(b_root_i, a_root_i);
		}
		else
		{
			JoinIntoSet(a_root_i, b_root_i);
			if (mItems[a_root_i].mRank == mItems[b_root_i].mRank)
				++mItems[b_root_i].mRank;
		}
	}
	template<class F>
	void ForEachRoot(const F& f)
	{
		for (auto& Item : mItems)
			if (Item.IsRoot())
				f(Item);
	}
	size_t GetRootCount() const { return mNumberOfRoots; }
	T& operator[](size_t i) { return mItems[i]; }
	const T& operator[](size_t i) const { return mItems[i]; }
	auto begin() { return mItems.begin(); }
	auto end() { return mItems.end(); }
	void clear() { mItems.clear(); mNumberOfRoots = 0; }
};

struct cDSUItem : public cIntrusiveDisjointSetsItemBase
{
	int idx;
	int size = 1;
	void SetJoined(cDSUItem& JoiningSet)
	{
		size += JoiningSet.size;
	}
};

using cDSU = tIntrusiveDisjointSets<cDSUItem>;



/*
struct cLine
{
    vector<ll> i;
    cStringVector s;
    string txt;
    bool is_empty = false;
    int idx;
};
*/

const char* firstPartName = "cikkek";

namespace
{

struct cInfo
{
    int idx;
	int refs = 0;
    std::string id;
    vector<int> pos_in_line; // -1: not found
    vector<int> lines_present;

    bool is_present_in_all_lines() const
    {
        return all_of(ALL(pos_in_line), [](int x) { return x != -1; });
    }
};

vector<cInfo> is;
unordered_map<string, int> ids;

struct cLineInfo
{
	vector<int> ids;
};

vector<cLineInfo> lines;

}


void solveFirst()
{
    is.clear();
    ids.clear();
	lines.clear();

    ll res = 0;
	
	lines.resize(ls.size());
    for (auto& l : ls)
    {
		auto& line = lines[l.idx];
        for (auto&& [pos_in_line, id] : vs::enumerate(l.s))
        {
            auto it = ids.find(id);
            if (it == ids.end())
            {
                auto& info = is.emplace_back();
                info.idx = is.size() - 1;
                info.id = id;
                info.pos_in_line.resize(ls.size(), -1);
                ids[id] = info.idx;
            }
            auto& info = is[ids[id]];
            if(info.pos_in_line[l.idx] !=-1)
            {
                P("duplicate id: %s in line %d", id.c_str(), l.idx);
                continue;
            }
            info.pos_in_line[l.idx] = pos_in_line;
            info.lines_present.push_back(l.idx);
			line.ids.push_back(info.idx);
        }



		auto cikk_id = l.s[0];
		for (auto&& [pos_in_line, id] : vs::enumerate(l.s))
		{
			if(pos_in_line== 0)
                continue;
			is[ids[id]].refs++;
		}

    }


	auto result = count_if(ALL(is), [](auto& info) { return info.refs == 0; });

	P("result: %d", result);




















    //for(auto& info : is)
    //{
    //    if(info.is_present_in_all_lines())
    //    {
    //        P("id: %s is present in all lines", info.id.c_str());
    //        res++;
    //    }
    //}
    //for (auto& info : is)
    //{
    //    int lines_present_in = 0;
    //    for (auto line : info.lines_present)
    //    {
    //        if (info.pos_in_line[line] != -1)
    //        {
    //            lines_present_in++;
    //        }
    //    }
    //    P("id: %s is present in %d lines", info.id.c_str(), lines_present_in);
    //}

//	cDSU dsu;
////	dsu.Init(is.size());
//	vector<cDSUItem> roots;
//	roots.resize(is.size());
//	for(int i = 0; i < is.size(); ++i)
//    {
//        roots[i].idx = i;
//    }
//	dsu.Init(std::move(roots));
//
//	for (auto& line : lines)
//	{
//		for (int i = 1; i < line.ids.size(); ++i)
//        {
//            dsu.Union(line.ids[0], line.ids[i]);
//        }
//	}
//	int root_count = 0;
//	std::vector<std::string> lone_root_ids;
//	dsu.ForEachRoot([&](auto& root)
//		{
//			++root_count;
//			if (root.size == 1)
//                lone_root_ids.push_back(is[root.idx].id);
//			else
//			{
//				P("root: %s, size: %d", is[root.idx].id.c_str(), root.size);
//			}
//		});
//
//	sort(ALL(lone_root_ids));
//	P("alpha first lone root: %s", lone_root_ids[0].c_str());
//
//    P("root count: %d", root_count);
//    P("result: %lld", res);
}

