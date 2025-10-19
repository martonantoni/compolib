ll binomialCoeff(ll n, ll k)
{
	vector<ll> C;
	C.resize(k+1, 0);

	C[0] = 1;  // nC0 is 1

	for(ll i = 1; i <= n; i++)
	{
	// Compute next row of pascal triangle using
	// the previous row
		for(ll j = min(i, k); j > 0; j--)
			C[j] = C[j] + C[j-1];
	}
	return C[k];
}

///-------------------- precalculated, for smallish n & k: 

constexpr ll max_pascal_n=1010;

ll pascal[(max_pascal_n+1)*(max_pascal_n+1)];

ll binomial(ll n, ll k)
{
	if(k==0||k==n)
		return 1;
	if(k==1||k==n-1)
		return n;
	return pascal[n*max_pascal_n+k];
}

void buildPascal()
{
	for(int n=0; n<=max_pascal_n; ++n)
	{
		ll* row=pascal+n*max_pascal_n;
		ll* prev_row=pascal+(n-1)*max_pascal_n;
		row[0]=1;
		row[n]=1;
		for(int k=1; k<n; ++k)
		{
			row[k]=(prev_row[k-1]+prev_row[k])%mod;
		}
	}
}



///----------------- FASTER, MUCH FASTER:


// https://comeoncodeon.wordpress.com/2011/07/31/combination/

/* This function calculates power of p in n! */
ll countFact(ll n, ll p)
{
    ll k = 0;
    while (n >= p)
    {
        k += n / p;
        n /= p;
    }
    return k;
}

/* This function calculates (a^b)%MOD */
long long pow(ll a, ll b, ll MOD)
{
    long long x = 1, y = a;
    while (b > 0)
    {
        if (b % 2 == 1)
        {
            x = (x * y);
            if (x > MOD) x %= MOD;
        }
        y = (y * y);
        if (y > MOD) y %= MOD;
        b /= 2;
    }
    return x;
}

/*  Modular Multiplicative Inverse
    Using Euler's Theorem
    a^(phi(m)) = 1 (mod m)
    a^(-1) = a^(m-2) (mod m) */
long long InverseEuler(ll n, ll MOD)
{
    return pow(n, MOD - 2, MOD);
}

long long factMOD(ll n, ll MOD)
{
    long long res = 1;
    while (n > 0)
    {
        for (ll i = 2, m = n % MOD; i <= m; i++)
            res = (res * i) % MOD;
        if ((n /= MOD) % 2 > 0)
            res = MOD - res;
    }
    return res;
}

long long C(ll n, ll r, ll MOD)
{
    if (countFact(n, MOD) > countFact(r, MOD) + countFact(n - r, MOD))
        return 0;

    return (factMOD(n, MOD) *
        ((InverseEuler(factMOD(r, MOD), MOD) *
            InverseEuler(factMOD(n - r, MOD), MOD)) % MOD)) % MOD;
}