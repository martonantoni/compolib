#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>

#include "FastFileReader.h"

#define THROW_DETAILED_EXCEPTION(x) DebugBreak();
void ThrowLastError()
{
    DebugBreak();
}
inline int Low32(const __int64 Value)
{
    return (*(const LARGE_INTEGER*)&Value).LowPart;
}

inline int High32(const __int64 Value)
{
    return (*(const LARGE_INTEGER*)&Value).HighPart;
}


cFastFileReader::cFastFileReader(const std::string& path) : FileName(path)
{
	FileHandle=::CreateFile(FileName.c_str(),GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	if(FileHandle==INVALID_HANDLE_VALUE)
		ThrowLastError();
	FileMappingHandle=::CreateFileMapping(FileHandle,NULL,PAGE_WRITECOPY,0,0,NULL);
	if(FileMappingHandle==INVALID_HANDLE_VALUE)
	{
		::CloseHandle(FileHandle);
		ThrowLastError();
	}
	GetFileSizeEx(FileHandle,(LARGE_INTEGER *)&mFileSize);
	SYSTEM_INFO SystemInfo;
	::GetSystemInfo(&SystemInfo);
	SystemGranuality=SystemInfo.dwAllocationGranularity;

	mViewOffset=0;
	mViewPosition=mPosition=mEndPosition=NULL;
}

cFastFileReader::~cFastFileReader()
{
	if(FileHandle!=INVALID_HANDLE_VALUE)
	{
		if(mViewPosition)
			if(!::UnmapViewOfFile(mViewPosition))
				ThrowLastError();
		if(FileMappingHandle!=INVALID_HANDLE_VALUE)
			CloseHandle(FileMappingHandle);
		CloseHandle(FileHandle);
	}
}

int cFastFileReader::MoveView()
{ 
	if(mViewPosition)
		if(!::UnmapViewOfFile(mViewPosition))
			ThrowLastError();
	__int64 Offset=(mPosition-mViewPosition)+mViewOffset;
	if(Offset==mFileSize)
	{
		mViewPosition=NULL;
		return false;
	}
	__int64 OffsetError=Offset%SystemGranuality;
	mViewOffset=Offset-OffsetError;
	int ViewSize=Low32(std::min<__int64>(mFileSize-mViewOffset,MaxViewSize));
	mViewPosition=(char *)::MapViewOfFile(FileMappingHandle,FILE_MAP_COPY,High32(mViewOffset),Low32(mViewOffset),ViewSize);
	if(!mViewPosition)
		ThrowLastError();
	mPosition=mViewPosition+OffsetError;
	mEndPosition=mViewPosition+ViewSize;
	return true;
}

// 0x0a
// 0x0d 0x0a

cFastFileReader::cLine cFastFileReader::GetNextLine()
{
    if (mPosition == mEndPosition)
    {
        if (!MoveView())
            return cLine(NULL, 0);
    }
    // Find the end of the line
    for (;;) // (try again if there was not enough buffer first)
    {
        char* lineEnd = mPosition;
        for (; lineEnd != mEndPosition; ++lineEnd)
        {
            if (*lineEnd <= 0xd)
            {
                if (*lineEnd == 0xa || *lineEnd == 0)
                    break;
            }
        }
        if (lineEnd == mEndPosition)
        { // Did not find the line's end
            __int64 OldViewOffset = mViewOffset;
            if (!MoveView())
                return cLine(NULL, 0); // End of file
            if (mViewOffset == OldViewOffset)
            {
                if (mViewOffset + (mEndPosition - mViewPosition) == mFileSize)
                {
                    --lineEnd;
                    char* LineStart = mPosition;
                    mPosition = mEndPosition;
                    return cLine(LineStart, (int)(lineEnd - LineStart + 1));
                }
                else
                    THROW_DETAILED_EXCEPTION(fmt::sprintf("Too long line in file (ViewOffset: %d). File: %s", mViewOffset, FileName.c_str()));
            }
        }
        else
        {
            char* lineStart = mPosition;
            mPosition = lineEnd + 1; // start of the next line
            int lineLength = static_cast<int>(lineEnd - lineStart);
            if (lineLength > 1 && lineStart[lineLength - 1] == 0xd)
                --lineLength;
            return cLine(lineStart, lineLength);
        }
    }
    return cLine(NULL, 0);
}
