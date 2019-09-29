int gcd(int a, int b)
{
	while(b) b ^= a ^= b ^= a %= b;
	return a;
}

ll lcm(ll a, ll b)
{
	return (a/gcd(a, b))*b;
}