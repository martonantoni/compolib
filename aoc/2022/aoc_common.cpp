#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

#include "aoc_common.h"

inline int Low32(const __int64 Value)
{
    return (*(const LARGE_INTEGER*)&Value).LowPart;
}

inline int High32(const __int64 Value)
{
    return (*(const LARGE_INTEGER*)&Value).HighPart;
}

cFastFileReader::cFastFileReader(const cPath& Path) : FileName(Path)
{
    FileHandle = ::CreateFile(FileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    if (FileHandle == INVALID_HANDLE_VALUE)
        ThrowLastError(fmt::sprintf("CreateFile(\"%s\")", FileName.c_str()));
    FileMappingHandle = ::CreateFileMapping(FileHandle, NULL, PAGE_WRITECOPY, 0, 0, NULL);
    if (FileMappingHandle == INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(FileHandle);
        ThrowLastError(fmt::sprintf("CreateFileMapping(\"%s\")", FileName.c_str()));
    }
    GetFileSizeEx(FileHandle, (LARGE_INTEGER*)&FileSize);
    SYSTEM_INFO SystemInfo;
    ::GetSystemInfo(&SystemInfo);
    SystemGranuality = SystemInfo.dwAllocationGranularity;

    ViewOffset = 0;
    ViewPosition = Position = EndPosition = NULL;
}

cFastFileReader::~cFastFileReader()
{
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        if (ViewPosition)
            if (!::UnmapViewOfFile(ViewPosition))
                ThrowLastError(fmt::sprintf("UnmapViewOfFile failed. File: %s", FileName.c_str()));
        if (FileMappingHandle != INVALID_HANDLE_VALUE)
            CloseHandle(FileMappingHandle);
        CloseHandle(FileHandle);
    }
}

int cFastFileReader::MoveView()
{
    if (ViewPosition)
        if (!::UnmapViewOfFile(ViewPosition))
            ThrowLastError(fmt::sprintf("UnmapViewOfFile failed. File: %s", FileName.c_str()));
    __int64 Offset = (Position - ViewPosition) + ViewOffset;
    if (Offset == FileSize)
    {
        ViewPosition = NULL;
        return false;
    }
    __int64 OffsetError = Offset % SystemGranuality;
    ViewOffset = Offset - OffsetError;
    int ViewSize = Low32(min<__int64>(FileSize - ViewOffset, MaxViewSize));
    ViewPosition = (char*)::MapViewOfFile(FileMappingHandle, FILE_MAP_COPY, High32(ViewOffset), Low32(ViewOffset), ViewSize);
    if (!ViewPosition)
        ThrowLastError(fmt::sprintf("MapViewOfFile failed (offset= %d, size: %d). File: %s", ViewOffset, ViewSize, FileName.c_str()));
    Position = ViewPosition + OffsetError;
    EndPosition = ViewPosition + ViewSize;
    return true;
}

cFastFileReader::cLine cFastFileReader::GetNextLine()
{
    // Find the real start of the line, skipping all line separators
    for (;;)
    {
        for (; Position != EndPosition; ++Position)
        {
            if (*Position > 0xd)
                break;
            if (*Position != 0xd && *Position != 0xa && *Position != 0)
                break;
        }
        if (Position == EndPosition)
        {
            if (!MoveView())
                return cLine(NULL, 0);
        }
        else
            break;
    }
    // Find the end of the line
    for (;;) // (try again if there was not enough buffer first)
    {
        char* LineEnd = Position;
        for (; LineEnd != EndPosition; ++LineEnd)
        {
            if (*LineEnd <= 0xd)
            {
                if (*LineEnd == 0xd || *LineEnd == 0xa || *LineEnd == 0)
                {
                    break;
                }
            }
        }
        if (LineEnd == EndPosition)
        { // Did not find the line's end
            __int64 OldViewOffset = ViewOffset;
            if (!MoveView())
                return cLine(NULL, 0); // End of file
            if (ViewOffset == OldViewOffset)
            {
                if (ViewOffset + (EndPosition - ViewPosition) == FileSize)
                {
                    --LineEnd;
                    char* LineStart = Position;
                    Position = EndPosition;
                    return cLine(LineStart, (int)(LineEnd - LineStart + 1));
                }
                else
                    THROW_DETAILED_EXCEPTION(fmt::sprintf("Too long line in file (ViewOffset: %d). File: %s", ViewOffset, FileName.c_str()));
            }

        }
        else
        {
            char* LineStart = Position;
            Position = LineEnd;
            return cLine(LineStart, (int)(LineEnd - LineStart));
        }
    }
    return cLine(NULL, 0);
}



namespace
{
    template<class T> void AddFields(T& Container, const std::string& SourceString, const std::string& Delimeters, int EmptyFieldsAllowed)
    {
        const char* SourcePos = SourceString.c_str();
        if (Delimeters.length() > 1)
        {
            for (;;)
            {
                const char* DelimeterPos = strpbrk(SourcePos, Delimeters.c_str());
                if (!DelimeterPos)
                    DelimeterPos = SourceString.c_str() + SourceString.length();
                if (EmptyFieldsAllowed || DelimeterPos - SourcePos > 0)
                    Container.push_back(std::string(SourcePos, (int)(DelimeterPos - SourcePos)));
                if (!*DelimeterPos)
                    break;
                SourcePos = DelimeterPos + 1;
            }
        }
        else
        {
            char DelimeterChar = Delimeters[0];
            for (;;)
            {
                const char* DelimeterPos = strchr(SourcePos, DelimeterChar);
                if (!DelimeterPos)
                    DelimeterPos = SourceString.c_str() + SourceString.length();
                if (EmptyFieldsAllowed || DelimeterPos - SourcePos > 0)
                    Container.push_back(std::string(SourcePos, (int)(DelimeterPos - SourcePos)));
                if (!*DelimeterPos)
                    break;
                SourcePos = DelimeterPos + 1;
            }
        }
    }
}

cStringVector::cStringVector(const std::string& SourceString, const std::string& Delimeters, bool EmptyFieldsAllowed)
{
    reserve(4);
    AddFields(*this, SourceString, Delimeters, EmptyFieldsAllowed);
}

void cStringVector::FromString(const std::string& SourceString, const std::string& Delimeters, bool EmptyFieldsAllowed)
{
    clear();
    reserve(4);
    AddFields(*this, SourceString, Delimeters, EmptyFieldsAllowed);
}

int cStringVector::FindIndex(const std::string& Token, int From) const
{
    for (int i = From, iend = (int)size(); i < iend; ++i)
        if ((*this)[i] == Token)
            return i;
    return -1;
}

cIntVector cStringVector::ToIntVector() const
{
    cIntVector IntVector;
    IntVector.resize(size());
    for (int i = 0, iend = (int)size(); i != iend; ++i)
        IntVector[i] = atoll((*this)[i].c_str());
    return IntVector;
}

cStringVector&& cStringVector::operator+(const std::string& ExtraField) const
{
    cStringVector ExtendedVector = *this;
    ExtendedVector.push_back(ExtraField);
    return std::move(ExtendedVector);
}