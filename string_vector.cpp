class cStringVector: public std::vector<std::string>
{
public:
	cStringVector() {}
	cStringVector(const std::string &SourceString, const std::string &Delimeters, bool EmptyFieldsAllowed=true);
	void FromString(const std::string &SourceString, const std::string &Delimeters, bool EmptyFieldsAllowed=true);
	int FindIndex(const std::string &Token, int From=0) const; // returns -1 if not found
	cStringVector &&operator+(const std::string &ExtraField) const;
};

namespace
{
	template<class T> void AddFields(T &Container, const std::string &SourceString, const std::string &Delimeters, int EmptyFieldsAllowed)
	{
		const char *SourcePos=SourceString.c_str();
		if(Delimeters.length()>1)
		{
			for(;;)
			{
				const char *DelimeterPos=strpbrk(SourcePos, Delimeters.c_str());
				if(!DelimeterPos)
					DelimeterPos=SourceString.c_str()+SourceString.length();
				if(EmptyFieldsAllowed||DelimeterPos-SourcePos>0)
					Container.push_back(std::string(SourcePos, (int)(DelimeterPos-SourcePos)));
				if(!*DelimeterPos)
					break;
				SourcePos=DelimeterPos+1;
			}
		}
		else
		{
			char DelimeterChar=Delimeters[0];
			for(;;)
			{
				const char *DelimeterPos=strchr(SourcePos, DelimeterChar);
				if(!DelimeterPos)
					DelimeterPos=SourceString.c_str()+SourceString.length();
				if(EmptyFieldsAllowed||DelimeterPos-SourcePos>0)
					Container.push_back(std::string(SourcePos, (int)(DelimeterPos-SourcePos)));
				if(!*DelimeterPos)
					break;
				SourcePos=DelimeterPos+1;
			}
		}
	}
}

cStringVector::cStringVector(const std::string &SourceString, const std::string &Delimeters, bool EmptyFieldsAllowed)
{
	reserve(4);
	AddFields(*this, SourceString, Delimeters, EmptyFieldsAllowed);
}

void cStringVector::FromString(const std::string &SourceString, const std::string &Delimeters, bool EmptyFieldsAllowed)
{
	clear();
	reserve(4);
	AddFields(*this, SourceString, Delimeters, EmptyFieldsAllowed);
}

int cStringVector::FindIndex(const std::string &Token, int From) const
{
	for(int i=From, iend=(int)size(); i<iend; ++i)
		if((*this)[i]==Token)
			return i;
	return -1;
}
