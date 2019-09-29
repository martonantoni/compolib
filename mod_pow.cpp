template <typename T>
T modpow(T base, T exp)
{
	base %= mod;
	T result = 1;
	while(exp > 0)
	{
		if(exp & 1) result = (result * base) % mod;
		base = (base * base) % mod;
		exp >>= 1;
	}
	return result;
}