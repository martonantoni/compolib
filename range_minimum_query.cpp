
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

const int MAX_LOG_N=17; ///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CHECK THIS!!!!!!

class cRangeMinimumQuery
{
	vector<vector<int>> mins;
public:
	void Init(const vector<int> &a)
	{
		int n=(int)a.size();
		mins.resize(MAX_LOG_N);
		for(auto &v: mins)
			v.resize(n);
		for(int i=0; i<n; ++i)
		{
			mins[0][i]=a[i];
		}
		for(int j=1, rs2=1; j<MAX_LOG_N; ++j, rs2<<=1)
		{
			for(int i=0; i<n; ++i)
			{
				if(i+rs2<n)
					mins[j][i]=min(mins[j-1][i], mins[j-1][i+rs2]);
				else
					mins[j][i]=mins[j-1][i];
			}
		}
	}
	int GetRMQ(int left, int right)
	{
		int range_size=right-left+1;
		int l32=log2_32(range_size);
		int queried_range_size=1<<l32, query_index=l32;
		return min(mins[query_index][left], mins[query_index][right-queried_range_size+1]);
	}
};

#ifdef _DEBUG

void TestRMQ()
{
	vector<int> a(5'000'0);
	std::mt19937 gen(4242442);
	std::uniform_int_distribution<int> num_generator(0, 5'000'0-1);
	for(auto &x: a)
		x=num_generator(gen);
	cRangeMinimumQuery rmq;
	rmq.Init(a);
	for(int tc=0;; ++tc)
	{
		int left=num_generator(gen), right=num_generator(gen);
		if(left>right)
			swap(left, right);
		int m=a[left];
		for(int i=left+1; i<=right; ++i)
			m=min(m, a[i]);
		ASSERT(m==rmq.GetRMQ(left, right));
	}
}

#endif