ll gcd(ll a, ll b)
{
	while(b) b ^= a ^= b ^= a %= b;
	return a;
}

ll lcm(ll a, ll b)
{
	return (a/gcd(a, b))*b;
}

ll lcm(const vector<ll>& nums)
{
    if (nums.empty())
        return 0;
    if (nums.size() == 1)
        return nums[0];
    ll res = lcm(nums[0], nums[1]);
    for (size_t i = 2; i < nums.size(); ++i)
        res = lcm(res, nums[i]);
    return res;
}
