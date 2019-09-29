bool IsPalindrome(const std::string &s, size_t LeftIndex, size_t RightIndex)
{
	while(LeftIndex<RightIndex)
	{
		if(s[LeftIndex]!=s[RightIndex])
			return false;
		++LeftIndex;
		--RightIndex;
	}
	return true;
}

vector<int> LetterFrequency(const std::string &s) // for lower case 'a'-'z'
{
	vector<int> f(26, 0);
	for(char x: s)
		++f[x-'a'];
	return f;
}

int RemoveRepetitions(char *p) // returns new length
{
	char *s=p, *t=p;
	while(*s)
	{
		while(*t==*s)
			++s;
		++t;
		*t=*s;
	}
	return (int)(t-p);
}