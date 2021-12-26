
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
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

#undef max
#undef min
#undef I

#ifdef __GNUC__ 
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

#ifdef _DEBUG
#define ASSERT(x) if(!(x)) __debugbreak()
#define LOG(...) fprintf(stderr, __VA_ARGS__)
#define D(expression) expression
#else
char* crash_please = (char*)42;
#define ASSERT(x) 
//if(!(x)) { printf("%s failed",#x); *crash_please=33; }
//#define LOG(...) fprintf(stderr, __VA_ARGS__)
#define LOG(...)
#define D(expression)
#endif

#include <chrono>

class cLogPerformance_Guard
{
    std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime = std::chrono::high_resolution_clock::now();
    const char* mName;
public:
    cLogPerformance_Guard(const char* Name) : mName(Name) {}
    ~cLogPerformance_Guard()
    {
        auto EndTime = std::chrono::high_resolution_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - mStartTime);
        LOG("\n--- Elapsed %llu.%llu ms in %s ---\n", (unsigned long long)Elapsed.count() / 1000, (unsigned long long)Elapsed.count() % 1000, mName);
    }
    void waypoint()
    {
        auto EndTime = std::chrono::high_resolution_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - mStartTime);
        LOG("+ %llu.%llu ms: ", (unsigned long long)Elapsed.count() / 1000, (unsigned long long)Elapsed.count() % 1000);
    }
} perf_guard("main");

using namespace std;
using namespace std::chrono;
using namespace std::string_literals;
using ull = unsigned long long;
using ll = long long;
#define RI(var_name) int var_name; scanf("%d", &var_name);
#define RIV(var_name, size) vector<int> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%d", &item_of_##var_name);
#define RII(var_name1, var_name2) int var_name1, var_name2; scanf("%d %d", &var_name1, &var_name2);
#define RIII(var_name1, var_name2, var_name3) int var_name1, var_name2, var_name3; scanf("%d %d %d", &var_name1, &var_name2, &var_name3);
#define RIIII(var_name1, var_name2, var_name3, var_name4) int var_name1, var_name2, var_name3, var_name4; scanf("%d %d %d %d", &var_name1, &var_name2, &var_name3, &var_name4);
#define RL(var_name) ll var_name; scanf("%lld", &var_name);
#define RLV(var_name, size) vector<ll> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%lld", &item_of_##var_name);
#define RLUV(var_name, size) vector<ull> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%llu", &item_of_##var_name);
#define RLL(var_name1, var_name2) ll var_name1, var_name2; scanf("%lld %lld", &var_name1, &var_name2);
#define RLLL(var_name1, var_name2, var_name3) ll var_name1, var_name2, var_name3; scanf("%lld %lld %lld", &var_name1, &var_name2, &var_name3);
#define RLLLL(var_name1, var_name2, var_name3, var_name4) ll var_name1, var_name2, var_name3, var_name4; scanf("%lld %lld %lld %lld", &var_name1, &var_name2, &var_name3, &var_name4);
#define RD(var_name) double var_name; scanf("%lf", &var_name);
#define RDV(var_name, size) vector<double> var_name(size); for(auto &item_of_##var_name: var_name) scanf("%lf", &item_of_##var_name);
#define RDD(var_name1, var_name2) double var_name1, var_name2; scanf("%lf %lf", &var_name1, &var_name2);
#define RDDD(var_name1, var_name2, var_name3) double var_name1, var_name2, var_name3; scanf("%lf %lf %lf", &var_name1, &var_name2, &var_name3);
#define ALL(cont) cont.begin(), cont.end()
#define FOR(var, max_value) for(decltype(max_value) var=0;var<max_value;++var)

constexpr ll mod = 1'000'000'007;

ll round_up_div(ll osztando, ll oszto)
{
    return (osztando + oszto - 1) / oszto;
}
ll gcd(ll a, ll b)
{
    while (b) b ^= a ^= b ^= a %= b;
    return a;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


using cPath = std::string;
#define ThrowLastError(...) DebugBreak()
#define THROW_DETAILED_EXCEPTION(...) DebugBreak()
inline int Low32(const __int64 Value)
{
    return (*(const LARGE_INTEGER*)&Value).LowPart;
}

inline int High32(const __int64 Value)
{
    return (*(const LARGE_INTEGER*)&Value).HighPart;
}

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

    cLine GetNextLine();
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

typedef std::vector<ll> cIntVector;


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

ll sum_of_ar_seq(ll first, ll step, ll count)   // count: how many items in total
{
    return (first + first + (count - 1) * step) * count / 2;
}

// ll Solve(const vector<ll>& raw)
// {
// }

/*

template<class T>
struct tPoint
{
    T x, y;

    constexpr tPoint() : x(0), y(0) {}
    constexpr tPoint(T _x, T _y) : x(_x), y(_y) {}
    template<class U> constexpr tPoint(const tPoint<U>& Other) : x(Other.x), y(Other.y) {}

    tPoint& operator-=(const tPoint& Other) { x -= Other.x; y -= Other.y; return *this; }
    tPoint& operator+=(const tPoint& Other) { x += Other.x; y += Other.y; return *this; }
    tPoint operator+(const tPoint& Offset) const { return tPoint(x + Offset.x, y + Offset.y); }
    tPoint operator-(const tPoint& Offset) const { return tPoint(x - Offset.x, y - Offset.y); }
    tPoint operator-() const { return tPoint(-x, -y); }
    tPoint operator/(const tPoint& Other) const { return tPoint(x / Other.x, y / Other.y); }
    template<class M> tPoint operator*(M m) const { return tPoint(x * m, y * m); }
    template<class D> tPoint operator/(D d) const { return tPoint(x / d, y / d); }
    template<class U> tPoint& operator=(const tPoint<U>& Other) { x = Other.x;  y = Other.y; }
    bool operator==(const tPoint& Other) const { return x == Other.x && y == Other.y; }
    bool operator!=(const tPoint& Other) const { return x != Other.x || y != Other.y; }
    float DistanceFrom(tPoint Other) const
    {
        int xdist = x - Other.x, ydist = y - Other.y;
        return sqrt(xdist * xdist + ydist * ydist);
    }
    void RatioKeeping_SetX(T nx) { y = y * nx / x; x = nx; }
    void RatioKeeping_SetY(T ny) { x = x * ny / y; y = ny; }
    void Normalize()
    {
        double l = sqrt(x * x + y * y);
        x /= l;
        y /= l;
    }
};

typedef tPoint<int> cPoint;
typedef tPoint<float> cFloatPoint;

template<class T>
inline cFloatPoint Lerp(T a, T b, float t)
{
    return { a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
}


struct cImage
{
    void loadFromStrings(const vector<string>& lines);

    int& at(cPoint p);
    
    cImage& operator-=(int offset);
    cImage& operator+=(int offset);

    template<class F> void forEach(F func) //  >>>  [&](cPoint p, int& element){ }
    {
        cPoint p(0, 0);
        for (auto& e : data)
        {
            func(p, e);
            if (++p.x == w)
            {
                ++p.y;
                p.x = 0;
            }
        }
    }
    template<class F, class O> void forOffsets(cPoint point, const O& offsets, F func)
    {
        for (auto& o : offsets)
        {
            cPoint p = point + o;
            if (p.x >= 0 && p.x < w && p.y >= 0 && p.y < h)
                func(p, at(p));
        }
    }
    template<class F> void forN4(cPoint p, F func) //  >>>  forN4(p, [&](cPoint p, int& element){ }
    {
        static const array<cPoint, 4> offsets = { { { -1, 0 }, { 1, 0}, { 0, -1}, { 0, 1} } };
        forOffsets(p, offsets, func);
    }
    template<class F> void forN8(cPoint p, F func) //  >>>  forN8(p, [&](cPoint p, int& element){ }
    {
        static const array<cPoint, 8> offsets = { { { -1, -1 }, { -1, 0}, { -1, 1}, { 0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} } };
        forOffsets(p, offsets, func);
    }

    vector<int> data;
    int w, h;
};

void cImage::loadFromStrings(const vector<string>& lines)
{
    if (lines.empty())
        return;

    h = (int)lines.size();
    w = (int)lines[0].length();
    data.resize(w * h);
    int pos = 0;
    for (auto& l : lines)
    {
        for (auto c : l)
        {
            data[pos] = c;
            ++pos;
        }
    }
}

int& cImage::at(cPoint p)
{
    return data[p.y * h + p.x];
}

cImage& cImage::operator-=(int offset)
{
    for (auto& e : data)
        e -= offset;
    return *this;
}

cImage& cImage::operator+=(int offset)
{
    for (auto& e : data)
        e += offset;
    return *this;
}
*/
//////////////////////////////////////////////////////////////////////////

using cPoint = array<int, 3>;

using cOrientation = array<int, 6>;

array<cOrientation, 48> orientations =
{ {
    { 0, 1, 2,  1,  1,  1 },
    { 0, 1, 2,  1,  1, -1 },
    { 0, 1, 2,  1, -1,  1 },
    { 0, 1, 2,  1, -1, -1 },
    { 0, 1, 2, -1,  1,  1 },
    { 0, 1, 2, -1,  1, -1 },
    { 0, 1, 2, -1, -1,  1 },
    { 0, 1, 2, -1, -1, -1 },

    { 0, 2, 1,  1,  1,  1 },
    { 0, 2, 1,  1,  1, -1 },
    { 0, 2, 1,  1, -1,  1 },
    { 0, 2, 1,  1, -1, -1 },
    { 0, 2, 1, -1,  1,  1 },
    { 0, 2, 1, -1,  1, -1 },
    { 0, 2, 1, -1, -1,  1 },
    { 0, 2, 1, -1, -1, -1 },

    { 1, 0, 2,  1,  1,  1 },
    { 1, 0, 2,  1,  1, -1 },
    { 1, 0, 2,  1, -1,  1 },
    { 1, 0, 2,  1, -1, -1 },
    { 1, 0, 2, -1,  1,  1 },
    { 1, 0, 2, -1,  1, -1 },
    { 1, 0, 2, -1, -1,  1 },
    { 1, 0, 2, -1, -1, -1 },

    { 1, 2, 0,  1,  1,  1 },
    { 1, 2, 0,  1,  1, -1 },
    { 1, 2, 0,  1, -1,  1 },
    { 1, 2, 0,  1, -1, -1 },
    { 1, 2, 0, -1,  1,  1 },
    { 1, 2, 0, -1,  1, -1 },
    { 1, 2, 0, -1, -1,  1 },
    { 1, 2, 0, -1, -1, -1 },
    
    { 2, 1, 0,  1,  1,  1 },
    { 2, 1, 0,  1,  1, -1 },
    { 2, 1, 0,  1, -1,  1 },
    { 2, 1, 0,  1, -1, -1 },
    { 2, 1, 0, -1,  1,  1 },
    { 2, 1, 0, -1,  1, -1 },
    { 2, 1, 0, -1, -1,  1 },
    { 2, 1, 0, -1, -1, -1 },

    { 2, 0, 1,  1,  1,  1 },
    { 2, 0, 1,  1,  1, -1 },
    { 2, 0, 1,  1, -1,  1 },
    { 2, 0, 1,  1, -1, -1 },
    { 2, 0, 1, -1,  1,  1 },
    { 2, 0, 1, -1,  1, -1 },
    { 2, 0, 1, -1, -1,  1 },
    { 2, 0, 1, -1, -1, -1 },
} };

struct cScanner
{
    vector<cPoint> points;
    bool fixed = false;
    int orientation = 0;
    cPoint position = { 0,0,0 };
};

vector<cScanner> scanners;
int num_scanners;

cPoint transform(cPoint p, int oi)
{
    cOrientation orientation = orientations[oi];
    return 
    { 
        p[orientation[0]]*orientation[3], 
        p[orientation[1]]*orientation[4],
        p[orientation[2]]*orientation[5] 
    };
}

vector<cPoint> diffs;

cPoint diff(const cPoint& a, const cPoint& b)
{
    return { a[0] - b[0], a[1] - b[1], a[2] - b[2] };
}

// bool operator<(const cPoint& a, const cPoint& b)
// {
//     return a < b;
// }

bool tryPair(int bi, int ci)
{
    cScanner& base = scanners[bi];
    cScanner& checked = scanners[ci];
    FOR(oi, 48)
    {
        diffs.clear();
        for (auto& p1 : base.points)
        {
            for (auto& p2 : checked.points)
            {
                diffs.emplace_back(diff(p1, transform(p2, oi)));
            }
        }
        sort(ALL(diffs));
    }
    return false;
}

ll Solve()
{
    scanners.reserve(30);
    cFastFileReader file("c:/projects/_competition/in.txt");
 //   vector<string> lines;
    for (auto line : file)
    {
//        cStringVector v(line, "-", false);
//        lines.emplace_back(line);
//        return Solve(v.ToIntVector());
        string l(line);
        if (l[1] == '-')
        {
            scanners.emplace_back();
            continue;
        }
        auto iv = cStringVector(l, ",", false).ToIntVector();
        cPoint p={ (int)iv[0], (int)iv[1], (int)iv[2] };
        scanners.back().points.emplace_back(p);
    }
    num_scanners = (int)scanners.size();
    scanners[0].fixed = true;

    for (int i = 1; i < num_scanners; ++i)
    {
        if (tryPair(0, i))
        {
            int alma = 2;
        }
    }
    return 0;
}



int Init()
{
    constexpr int t = 1;
    //RI(t);
    return t;
}

int main()
{
    int t = Init();
    for (int tc = 1; tc <= t; ++tc)
    {
 //       printf("Case #%d: ", tc);
        ll result = Solve();
        printf("result: %lld\n", result);
        //         RI(verdict);
        //         if (verdict == -1)
        //             break;
    }
}






// void print(vector<int>& n)
// {
//     bool par = false;
//     for (auto x : n)
//     {
//         if (x == -1)
//             printf("[");
//     }
// }

// struct cNumber
// {
//     int value;
//     unique_ptr<cNumber> left, right;
//     bool is_real;
//     int depth;
// };
// 
// using up = unique_ptr<cNumber>;
// 
// const char* p;
// 
// unique_ptr<cNumber> read(int depth)
// {
//     up n = make_unique<cNumber>();
//     n->depth = depth;
//     if (*p == '[')
//     {
//         n->is_real = false;
//         ++p;
//         n->left = read(depth+1);
//         ++p; // ,
//         n->right = read(depth+1);
//         ++p; // ]
//     }
//     else
//     {
//         n->is_real = true;
//         n->value = *p - '0';
//         ++p;
//     }
//     return n;
// }
// 
// void reduce(cNumber& n)
// {
// 
// }