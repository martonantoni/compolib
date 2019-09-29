https://www.youtube.com/watch?v=tYzNrCul5OU

https://www.hackerrank.com/challenges/common-child


string a, b;

int cache[5000*5000];

void Init()
{
	for(int i=0; i<5000*5000; ++i)
		cache[i]=-1;
}

int LCS(int ai, int bi)
{
	if(ai==-1||bi==-1)
		return 0;
	int &cached_value=cache[ai*5000+bi];
	if(cached_value!=-1)
		return cached_value;
	if(a[ai]==b[bi])
		return cached_value=LCS(ai-1, bi-1)+1;
	return cached_value=max(LCS(ai-1, bi), LCS(ai, bi-1));
}

void Solve()
{
	cin >> a >> b;
	printf("%d", LCS(a.size()-1, b.size()-1));
}

int main()
{
	Init();
	int t=1;
	scanf("%d", &t);
	for(int tc=1; tc<=t; ++tc)
	{
//		Solve(tc);
		Solve();
	}
}

