ll mod_substract(ll x, ll y, ll m)
{
    return ((x - y) % m) + ((x >= y) ? 0 : m);
}

ll modpow(ll base, ll exp)
{
	base %= mod;
	ll result = 1;
	while(exp > 0)
	{
		if(exp & 1) result = (result * base) % mod;
		base = (base * base) % mod;
		exp >>= 1;
	}
	return result;
}
