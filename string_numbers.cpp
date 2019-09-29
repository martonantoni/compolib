namespace StringNumbers
{
	std::string WithoutLeadingZeroes(const std::string &a)
	{
		if(a.size()<=1)
			return a;
		auto i=std::find_if(a.begin(), a.end(), [](auto x) { return x!='0'; });
		if(i==a.end())
			return "0"s;
		return a.substr(i-a.begin());
	}

	int NumberAtPosition(const std::string &a, int p) // position is from the right
	{
		if(a.size()<=p)
			return 0;
		return a[a.size()-p-1]-'0';
	}

	std::string Add(const std::string &a, const std::string &b)
	{
		std::string r;
		int s=max((int)a.size(), (int)b.size());
		r.resize(s+1);
		int carry=0;
		for(int i=0; i<=s; ++i)
		{
			int char_result=NumberAtPosition(a, i)+NumberAtPosition(b, i)+carry;
			if(char_result>=10)
			{
				carry=1;
				char_result-=10;
			}
			else
				carry=0;
			r[s-i]='0'+char_result;
		}
		r[0]=carry+'0';
		return r;
	}

	std::string Multiply_Max10(const std::string &a, int b)
	{
		ASSERT(b<=10&&b>=0);
		if(b==10)
		{
			return a+'0';
		}
		if(b==1)
		{
			return a;
		}
		if(b==0)
		{
			return "0"s;
		}
		std::string r;
		int s=(int)a.size();
		r.resize(s+1);
		int carry=0;
		for(int i=0; i<s; ++i)
		{
			int char_result=NumberAtPosition(a, i)*b+carry;
			carry=char_result/10;
			char_result%=10;
			r[s-i]=char_result+'0';
		}
		r[0]=carry+'0';
		return r;
	}

 	std::string Multiply(const std::string &a, const std::string &b)
	{
		std::string r;
		int s=(int)b.size();
		r.reserve(max(a.size(), b.size())+1);
		for(int i=0; i<s; ++i)
		{
			auto sub_result=Multiply_Max10(a, b[i]-'0');
			r=Multiply_Max10(r, 10);
			r=WithoutLeadingZeroes(Add(r, sub_result));
		}
		return r;
 	}

}
