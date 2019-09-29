const int tab32[32] = {
	0,  9,  1, 10, 13, 21,  2, 29,
	11, 14, 16, 18, 22, 25,  3, 30,
	8, 12, 20, 28, 15, 17, 24,  7,
	19, 27, 23,  6, 26,  5,  4, 31 };

int log2_32(uint32_t value)
{
	value |= value >> 1;
	value |= value >> 2;
	value |= value >> 4;
	value |= value >> 8;
	value |= value >> 16;
	return tab32[(uint32_t)(value*0x07C4ACDD) >> 27];
}

class cSuffixArray
{
	std::vector<vector<int>> mSuffixRanks; // outer indexed by level, inner indexed by starting index
	std::vector<int> mOrderedSuffixIndexes;
	std::vector<int> mLCPArray;
public:
	cSuffixArray() {}
	void Build(const char *s, int length);
	void Build(const std::string &s) { Build(s.c_str(), (int)s.size()); }
	void BuildLCPArray(const char *s);
	int GetSuffixRank(int start_offset) const { return mSuffixRanks.back()[start_offset]; }

	void DebugDump(const char *s); // s: the string that was used for building

	void Solve(ll k);
};

void cSuffixArray::Build(const char *s, int length)
{
	mSuffixRanks.reserve(log2_32(length)+1);
	std::vector<int> level_rank(length);
	std::vector<int> indexes(length);
	for(int i=0; i<length; ++i)
		level_rank[i]=s[i]-'a';  /// <---------------------------------------------------------   ***  check alphabet! ***
	mSuffixRanks.emplace_back(std::move(level_rank));
	for(int checked_length=2; checked_length>>1<length; checked_length<<=1) // until prev. checked_length was long enough to cover entire string
	{
		int second_part_offset=checked_length>>1;
		std::iota(indexes.begin(), indexes.end(), 0);
		auto &prev_level_ranks=mSuffixRanks.back();
		std::sort(indexes.begin(), indexes.end(), [&prev_level_ranks, length, second_part_offset](int index_a, int index_b)
		{
			if(prev_level_ranks[index_a]<prev_level_ranks[index_b])
				return true;
			if(prev_level_ranks[index_a]>prev_level_ranks[index_b])
				return false;
			int next_level_rank_a=index_a+second_part_offset>=length?-1:prev_level_ranks[index_a+second_part_offset];
			int next_level_rank_b=index_b+second_part_offset>=length?-1:prev_level_ranks[index_b+second_part_offset];
			return next_level_rank_a<next_level_rank_b;
		});
		level_rank.resize(length);
		level_rank[indexes[0]]=0;
		auto RankAt=[&prev_level_ranks, length](int index)
		{
			return index>=length?-1:prev_level_ranks[index];
		};
		for(int i=1, rank_counter=0; i<length; ++i)
		{
			int checked_index=indexes[i], prev_index=indexes[i-1];
			if(prev_level_ranks[checked_index]==prev_level_ranks[prev_index]&&RankAt(checked_index+second_part_offset)==RankAt(prev_index+second_part_offset))
			{
				level_rank[checked_index]=level_rank[prev_index];
			}
			else
			{
				level_rank[checked_index]=++rank_counter;
			}
		}
		mSuffixRanks.emplace_back(std::move(level_rank));
	}
	mOrderedSuffixIndexes=std::move(indexes);
}

void cSuffixArray::BuildLCPArray(const char *s)
{
	int n=(int)mOrderedSuffixIndexes.size();
	int k=0;
	mLCPArray.resize(n, 0);
	auto &ranks=mSuffixRanks.back();
	for(int i=0; i<n; i++, k?k--:0)
	{
		if(ranks[i]==n-1)
		{ 
			k=0; continue; 
		}
		int j=mOrderedSuffixIndexes[ranks[i]+1];
		while(i+k<n && j+k<n && s[i+k]==s[j+k]) k++;
		mLCPArray[ranks[i]]=k;
	}
}

void cSuffixArray::DebugDump(const char *s)
{
	auto &ranks=mSuffixRanks.back();
	printf("-- ranks --\n");
	for(size_t i=0; i<ranks.size(); ++i)
	{
		printf("%d %.*s: %d\n", (int)i, (int)(ranks.size()-i), s+i, ranks[i]);
	}
	printf("\n-- suffixes in order of rank --\n");
	for(size_t i=0; i<ranks.size(); ++i)
	{
		int index=mOrderedSuffixIndexes[i];
		printf("%d %.*s: %d\n", index, (int)(ranks.size()-index), s+index, ranks[index]);
	}
	if(!mLCPArray.empty())
	{
		printf("\n-- LCPArray --\n");
		for(size_t i=0; i<ranks.size(); ++i)
		{
			int index=mOrderedSuffixIndexes[i];
			printf("%s: %d\n", s+index, mLCPArray[i]);
		}
	}
	printf("-----------------------------------------\n\n");
}

char buffer[100001];

void cSuffixArray::Solve(ll k)
{
	ll slen=(ll)mOrderedSuffixIndexes.size();
	ll last_lcp=0;
	for(ll i=0;i<slen;++i)
	{
		ll suffix_start_index=mOrderedSuffixIndexes[i];
		ll suffix_len=slen-suffix_start_index;
		ll first_added_len=last_lcp+1;
		ll last_added_len=suffix_len;
		ll this_adds=(first_added_len+last_added_len)*(last_added_len-first_added_len+1)/2;
		if(k-this_adds<=0)
		{
			//gotcha!
			for(ll i=first_added_len; i<=last_added_len; ++i)
			{
				if(k-i<=0)
				{
					printf("%c\n", buffer[suffix_start_index+k-1]);
					return;
				}
				k-=i;
			}
		}
		k-=this_adds;
		last_lcp=mLCPArray[i];
	}
}

void Solve()
{
	scanf("%s", buffer);
	int len=(int)strlen(buffer);
	cSuffixArray sa;
	sa.Build(buffer, len);
	sa.BuildLCPArray(buffer);
//	sa.DebugDump(buffer);
//	for(ll i=1;i<50;++i)
	RL(i);
	sa.Solve(i);
}