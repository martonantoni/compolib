#pragma once

#include <string>

class cFastFileReader
{
public:
	struct cLine
	{
		const char *Data = nullptr;
		int Length = 0;
		cLine(const char *pData,int pLength): Data(pData), Length(pLength) {}
        cLine() = default;
		int IsValid() { return Data!=NULL; }
		inline operator std::string() const { return std::string(Data,Length); }
		const char *end() const { return Data+Length; }
	};
    class iterator
    {
    public:
        iterator(cFastFileReader& reader, cLine line) : mReader(reader), mLine(line) {}
        cLine operator*() const { return mLine; }
        iterator& operator++() { mLine = mReader.GetNextLine(); return *this; }
        bool operator!=(const iterator& rhs) const { return mLine.Data != rhs.mLine.Data; }
    private:
        cFastFileReader& mReader;
        cLine mLine;
    };
private:
	std::string FileName; // Stored for debug purposes
	enum { MaxViewSize = 0x200000 }; 
	HANDLE FileHandle,FileMappingHandle;
	int SystemGranuality; // Size of memory pages
	__int64 mFileSize;
	__int64 mViewOffset;
	char *mViewPosition;
	char *mPosition;
	char *mEndPosition;
	int MoveView(); // returns true if there is more of the file to read
public:
	cFastFileReader(const std::string &path);
	~cFastFileReader();

	cLine GetNextLine();
	__int64 GetFileSize() const { return mFileSize; }
	const std::string &GetFileName() const { return FileName; }
    iterator begin() { return { *this, GetNextLine() }; }
    iterator end() { return { *this, {} }; }
};

inline const char *cbegin(const cFastFileReader::cLine &Line)
{
	return Line.Data;
}
inline const char *cend(const cFastFileReader::cLine &Line)
{
	return Line.end();
}
inline const char *begin(cFastFileReader::cLine &Line)
{
	return Line.Data;
}
inline const char *end(cFastFileReader::cLine &Line)
{
	return Line.end();
}
