template<class T> vector<T> rotate_left_n(const vector<T> &v, ll n)
{
	n%=(ll)v.size();
	if(n==0)
		return v;
	vector<T> rotated;
	rotated.resize(v.size());
	for(size_t i=v.size()-1, si=n-1; ; --i)
	{
		rotated[i]=v[si];
		if(si==0)
			si=v.size()-1;
		else
			--si;
		if(i==0)
			break;
	}
	return rotated;
}
