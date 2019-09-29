// https://www.hackerrank.com/challenges/longest-increasing-subsequent

// best help: http://stackoverflow.com/questions/2631726/how-to-determine-the-longest-increasing-subsequence-using-dynamic-programming

// also (but this is slower): http://www.geeksforgeeks.org/longest-monotonically-increasing-subsequence-size-n-log-n/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <set>
#include <tuple>
#include <cstring>
#include <deque>

#undef max
#undef min

#ifndef _MSC_VER 
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

#ifdef _DEBUG
#define ASSERT(x) if(!(x)) __debugbreak()
#else
#define ASSERT(x) if(!(x)) for(;;)
#endif

using namespace std;
using namespace std::string_literals;
using ull=unsigned long long;
using ll=long long;
constexpr ll mod=1'000'000'007;

#define RI(var_name) int var_name; scanf("%d", &var_name);
#define RIV(var_name, size) vector<int> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%d", &item_of_##var_name);
#define RII(var_name1, var_name2) int var_name1, var_name2; scanf("%d %d", &var_name1, &var_name2);
#define RIII(var_name1, var_name2, var_name3) int var_name1, var_name2, var_name3; scanf("%d %d %d", &var_name1, &var_name2, &var_name3);
#define RL(var_name) ll var_name; scanf("%lld", &var_name);
#define RLV(var_name, size) vector<ll> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%lld", &item_of_##var_name);
#define RLL(var_name1, var_name2) ll var_name1, var_name2; scanf("%lld %lld", &var_name1, &var_name2);
#define RLLL(var_name1, var_name2, var_name3) ll var_name1, var_name2, var_name3; scanf("%lld %lld %lld", &var_name1, &var_name2, &var_name3);
#define ALL(cont) cont.begin(), cont.end()
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Solve()
{
	RI(n);
	RIV(a, n);
	vector<int> best_end_for_length;
	best_end_for_length.reserve(n);
	best_end_for_length.push_back(a[0]);
	for(int i=1; i<n; ++i)
	{
		if(a[i]<best_end_for_length[0])
		{
			// smaller than the 1 length (0 index), this is the new one
			best_end_for_length[0]=a[i];
		}
		else if(a[i]>best_end_for_length.back())
		{
			// bigger than the end of the longest, this is the new longest ending
			best_end_for_length.push_back(a[i]);
		}
		else
		{
		// find the longest for which this can be added to:
		
			auto j2=lower_bound(ALL(best_end_for_length), a[i]);
			*j2=a[i];
			
		// this works also:
		// ... note that best_end_for_length is ordered, with the highest number at the back
		// and that   if(a[i]<best_end_for_length[j+1])  is always true
			
// 			for(int j=(int)best_end_for_length.size()-2; j>=0; --j)
// 			{
// 				if(a[i]>best_end_for_length[j])
// 				{
// 					// so, the one ending at best_end_for_length[j] (j+1 long) could be extended with a[i]. this is either
// 					// better than the next length best_end_for_length[j+1], or not. in the former case: replace it, otherwise, throw away a[i]
// 					if(a[i]<best_end_for_length[j+1])
// 						best_end_for_length[j+1]=a[i];
// 				}
// 			}
		}
	}
	printf("%d", (int)best_end_for_length.size());
}

int main()
{
	int t=1;
	//	scanf("%d", &t);
	for(int tc=1; tc<=t; ++tc)
	{
		Solve();
	}
}