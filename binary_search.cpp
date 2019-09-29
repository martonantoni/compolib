// Hivasnal nagyon figyelni, hogy a "to" az mar nem lehetseges ertek kell hogy legyen

#define higher true
#define lower false
template<class VT, class P>
VT value_lower_bound(VT from, VT to, const P &p)
{
	for(;;)
	{
		if(from==to)
			return from-1;
		VT mid=from+(to-from)/2;
		bool result=p(mid);
#ifdef _DEBUG
		printf("trying %lld, result: %s\n", mid, result==higher?"higher":"lower");
#endif
		if(result==higher)
		{
			from=mid+1;
		}
		else
		{
			to=mid;
		}
	}
}


void TestBinarySearch()
{
	for(ll t=0;t<1000000ll;++t)
	{
		ll lbt=value_lower_bound(0ll, 1000000ll, [t](auto x) { return x<=t; });
		ASSERT(lbt==t);
	}
}


template<class P>
int find_first_where_true(int first, int last, const P& p)
{
	for(;;)
	{
		if(first==last)
			return first;
		int mid = first + (last-first)/2;
		bool result = p(mid);
		if(!result)
		{
			// for 'mid' it is not true, must be after it
			first=mid+1;
		}
		else
		{
			// for 'mid' it is true, no use looking for start after it, but it might start at 'mid'.
			last=mid;
		}
	}
}
