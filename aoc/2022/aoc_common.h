#pragma once


#include <iostream>
#include <stdio.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <string>
#include <set>
#include <tuple>
#include <functional>
#include <cstring>
#include <deque>
#include <array>
#include <map>
#include <random>
#include <unordered_map>
#include <cstddef>
#include <cinttypes>
#include <regex>
#include <string>
#include <type_traits>

using namespace std;
using namespace std::string_literals;
using ull = unsigned long long;
using ll = long long;

inline ll divRoundUp(ll a, ll b) { return (a + b - 1) / b; }

using cIntVector = std::vector<ll>;

class cStringVector : public std::vector<std::string> // could be in its separate header file, but I don't care.
{
public:
    cStringVector() {}
    cStringVector(const std::string& SourceString, const std::string& Delimeters, bool EmptyFieldsAllowed = true);
    void FromString(const std::string& SourceString, const std::string& Delimeters, bool EmptyFieldsAllowed = true);
    int FindIndex(const std::string& Token, int From = 0) const; // returns -1 if not found
    cIntVector ToIntVector() const;
    cStringVector&& operator+(const std::string& ExtraField) const;
};

//////////////////////////////////////////////////////////////////////////

std::string replaceAll(const std::string& original, const std::string& subStringToMatch, const std::string replacement);

//////////////////////////////////////////////////////////////////////////

using cPath = std::string;
#define ThrowLastError(...) DebugBreak()
#define THROW_DETAILED_EXCEPTION(...) DebugBreak()

class cFastFileReader
{
public:
    struct cLine
    {
        const char* Data = nullptr;
        int Length = 0;
        cLine(const char* pData, int pLength) : Data(pData), Length(pLength) {}
        cLine() = default;
        int IsValid() { return Data != NULL; }
        inline operator std::string() const { return std::string(Data, Length); }
        const char* end() const { return Data + Length; }
        cStringVector toStringVector(const std::string& Delimeters, bool EmptyFieldsAllowed = true) const
        {
            return cStringVector(std::string(Data, Length), Delimeters, EmptyFieldsAllowed);
        }
        void copyTo(char* dest) { memcpy(dest, Data, Length); }
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
    enum { MaxViewSize = 0x20000 };
    HANDLE FileHandle, FileMappingHandle;
    int SystemGranuality; // Size of memory pages
    __int64 FileSize;
    __int64 ViewOffset;
    char* ViewPosition;
    char* Position;
    char* EndPosition;
    int MoveView(); // returns true if there is more of the file to read
public:
    cFastFileReader(const cPath& Path);
    ~cFastFileReader();

    cLine GetNextLine();   // SKIPS EMPTY LINES!
    __int64 GetFileSize() const { return FileSize; }
    const std::string& GetFileName() const { return FileName; }
    iterator begin() { return { *this, GetNextLine() }; }
    iterator end() { return { *this, {} }; }
};

inline const char* cbegin(const cFastFileReader::cLine& Line)
{
    return Line.Data;
}
inline const char* cend(const cFastFileReader::cLine& Line)
{
    return Line.end();
}
inline const char* begin(cFastFileReader::cLine& Line)
{
    return Line.Data;
}
inline const char* end(cFastFileReader::cLine& Line)
{
    return Line.end();
}

//////////////////////////////////////////////////////////////////////////


struct cPosition
{
    int row, col;

    cPosition() = default;
    constexpr cPosition(int r, int c) : row(r), col(c) {}
    constexpr cPosition(const cPosition& src) : row(src.row), col(src.col) {}
    auto operator<=>(const cPosition&) const = default;
    cPosition& operator+=(const cPosition& p)
    {
        row += p.row;
        col += p.col;
        return *this;
    }
    bool isWithinBounds(int w, int h) const 
    {
        return row >= 0 && row < h&& col >= 0 && col < w;
    }
    const int& operator[](int i) const { return i == 0 ? row : col; }
    int& operator[](int i) { return i == 0 ? row : col; }
};

template<> struct hash<cPosition>
{
    size_t operator()(const cPosition& p) const { return (p.row << 16) ^ p.col; };
};

constexpr cPosition operator+(const cPosition& l, const cPosition& r)
{
    return { l.row + r.row, l.col + r.col };
}

constexpr static cPosition direction_N = { -1, 0 };                     //         N
constexpr static cPosition direction_S = { 1, 0 };                      //         ^
constexpr static cPosition direction_E = { 0, 1 };                      //         |
constexpr static cPosition direction_W = { 0, -1 };                     //     W <-+-> E
constexpr static cPosition direction_NE = direction_N + direction_E;    //         |
constexpr static cPosition direction_NW = direction_N + direction_W;    //         V
constexpr static cPosition direction_SE = direction_S + direction_E;    //         S
constexpr static cPosition direction_SW = direction_S + direction_W;

constexpr static array<cPosition, 4> neighbour4Positions =  // in clockwise order
{ {
//     ^  0          >   1       V   2        <  3
    direction_N, direction_E, direction_S, direction_W
} };

constexpr static array<cPosition, 8> neighbour8Positions = // in clockwise order
{ {
    direction_N, direction_NE, direction_E, direction_SE, direction_S, direction_SW, direction_W, direction_NW
} };


template<class DATA_TYPE>
struct cImage
{
    int w=0, h=0;
    vector<char> cells;
    vector<DATA_TYPE> data;

    char& at(const cPosition& pos)
    {
        return cells[w * pos.row + pos.col];
    }
    DATA_TYPE& data_at(const cPosition& pos)
    {
        return data[w * pos.row + pos.col];
    }

    void load();

    bool isValidPos(const cPosition& pos);
// [&](cPosition pos)
    template<class T> void forEach4Neighbour(cPosition pos, const T& f)
    {
        for (auto offset : neighbour4Positions)
        {
            cPosition neighbourPos = pos + offset;
            if (isValidPos(neighbourPos))
            {
                f(neighbourPos);
            }
        }
    }
// [&](cPosition pos)
    template<class T> void forAll(const T& f)
    {
        for (int r = 0; r < h; ++r)
        {
            for (int c = 0; c < w; ++c)
            {
                f(cPosition(r, c));
            }
        }
    }
};

template<class DATA_TYPE>
inline bool cImage<DATA_TYPE>::isValidPos(const cPosition& pos)
{
    return pos.row >= 0 && pos.row < h&& pos.col >= 0 && pos.col < w;
}

template<class DATA_TYPE>
inline void cImage<DATA_TYPE>::load()
{
    cFastFileReader in("in.txt");
    cells.reserve(in.GetFileSize());
    int size = 0;
    for (auto line : in)
    {
        if (!w)
            w = line.Length;
        else
            ASSERT(w == line.Length);
        ++h;
        cells.resize(size + w);
        memcpy(cells.data() + size, line.Data, w);
        size += w;
    }
    data.resize(cells.size());
}
