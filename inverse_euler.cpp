/* This function calculates (a^b)%MOD */
ll pow(ll a, ll b, ll MOD)
{
	ll x=1, y=a;
	while(b > 0)
	{
		if(b%2 == 1)
		{
			x=(x*y);
			if(x>MOD) x%=MOD;
		}
		y = (y*y);
		if(y>MOD) y%=MOD;
		b /= 2;
	}
	return x;
}

/*  Modular Multiplicative Inverse
Using Euler's Theorem
a^(phi(m)) = 1 (mod m)
a^(-1) = a^(m-2) (mod m) */

unordered_map<ll, ll> ie_cache;

ll InverseEuler(ll n, ll MOD)
{
	ll& cached=ie_cache[n];
	if(cached)
		return cached;
	return cached=pow(n, MOD-2, MOD);
}