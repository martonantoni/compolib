
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

const int p1start = 7, p2start = 6; // my
//const int p1start = 4, p2start = 8; // test

// struct cResult
// {
//     ll rolled = 0;
//     ll winner_score = 0;
//     ll loser_score = 0;
//     int winner = 1, loser = 0;
//     ll scores[2] = { 0,0 };
//     ll position[2];
// };
// 
// cResult simulate(ll dice_size, ll board_size, ll target_score)
// {
//     cResult result;
//     result.position[0] = p1start -1;
//     result.position[1] = p2start -1;
//     while (result.scores[result.winner] < target_score)
//     {
//         swap(result.winner, result.loser);
//         ll steps = 0;
//         steps += (result.rolled + 1) % dice_size;
//         steps += (result.rolled + 2) % dice_size;
//         steps += (result.rolled + 3) % dice_size;
//         result.position[result.winner] += steps;
//         result.position[result.winner] %= board_size;
//         result.scores[result.winner] += result.position[result.winner] + 1;
// 
//         result.rolled += 3;
//     }
//     result.loser_score = result.scores[result.loser];
//     result.winner_score = result.scores[result.loser];
//     ++result.position[0];
//     ++result.position[1];
//     return result;
// }

vector<pair<ll, ll>> rolls;

//using cPlayerState = array<array<ll, 32>, 10>;

// using cPlayerState = array<ll, 320>;
// 
// ll& ways(cPlayerState& state, int position, int score)
// {
//     return state[position * 32 + score];
// }

using cState = array<ll, 32 * 32 * 16 * 16>;

ll& ways0(cState& state, int p0p, int p0s, int p1p, int p1s)
{
    return state[p0s * 32 * 16 * 16 + p1s * 16 * 16 + p0p * 16 + p1p];
}

ll& ways1(cState& state, int p1p, int p1s, int p0p, int p0s)
{
    return state[p0s * 32 * 16 * 16 + p1s * 16 * 16 + p0p * 16 + p1p];
}

cState state;
cState target_state;

void simulate()
{
    rolls.resize(7);
    for (int i = 3; i <= 9; ++i)
    {
        rolls[i - 3].first = i;
        rolls[i - 3].second = 0;
    }
    for (int i = 1; i <= 3; ++i)
    {
        for (int j = 1; j <= 3; ++j)
        {
            for (int k = 1; k <= 3; ++k)
            {
                ++rolls[i + j + k - 3].second;
            }
        }
    }
    fill(ALL(state), 0ll);
    ways0(state, p1start - 1, 0, p2start - 1, 0) = 1;
    ll wins[2] = { 0, 0 };
    int next = 0;

    for(int i=0;i<100;++i)
    {
        fill(ALL(target_state), 0ll);
        for (auto& r : rolls)
        {
            for (int source_pos = 0; source_pos < 10; ++source_pos)
            {
                int lands_on = (source_pos + r.first) % 10;
                for (int other_pos = 0; other_pos < 10; ++other_pos)
                {
                    for (int source_score = 0; source_score < 21; ++source_score)
                    {
                        int score = source_score + lands_on + 1;
                        for (int other_score = 0; other_score < 21; ++other_score)
                        {
                            if (next == 0)
                            {
                                ways0(target_state, lands_on, score, other_pos, other_score) += ways0(state, source_pos, source_score, other_pos, other_score) * r.second;
                            }
                            else
                            {
                                ways1(target_state, lands_on, score, other_pos, other_score) += ways1(state, source_pos, source_score, other_pos, other_score) * r.second;
                            }
                        } // other_score
                    } // source_score
                } // other_pos
            } // source_pos
        }

        for (int score = 21; score <= 30; ++score)
        {
            for (int pos = 0; pos < 10; ++pos)
            {
                for (int other_score = 0; other_score < 21; ++other_score)
                {
                    for (int other_pos = 0; other_pos < 10; ++other_pos)
                    {
                        wins[0] += ways0(target_state, pos, score, other_pos, other_score);
                        wins[1] += ways1(target_state, pos, score, other_pos, other_score);
                    }
                }
            }
        }
        state = target_state;
        next = (++next) % 2;
    }
    printf("p1 wins: %lld, p2 wins: %lld\n", wins[0], wins[1]);
}


ll Solve()
{
    cFastFileReader file("c:/projects/_competition/in.txt");
//    vector<string> lines;

//     for (auto line : file)
//     {
// //        cStringVector v(line, "-", false);
// //        lines.emplace_back(line);
// //        return Solve(v.ToIntVector());
//     }

//    auto r=simulate(100, 10, 1000);

//    return r.scores[r.loser] * r.rolled;
    simulate();
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


/*


cPlayerState& source = ps[next];
cPlayerState& other = ps[(next + 1) % 2];
cPlayerState target;
memset(&target, 0, sizeof(target));

ll lost_on_this = 0;
for (auto& r : rolls)
{
    for (int source_pos = 0; source_pos < 10; ++source_pos)
    {
        int target_pos = (r.first + source_pos) % 10;
        int extra_score = target_pos + 1;
        for (int source_score = 0; source_score < 21; ++source_score)
        {
            if (source_score + extra_score >= 21)
            {
                ll winners = ways(source, source_pos, source_score);
                wins[next] += winners * r.second;
                lost_on_this += winners;
            }
            else
            {
                ways(target, target_pos, source_score + extra_score) += ways(source, source_pos, source_score) * r.second;
            }
        }
    }
}
if (lost_on_this > 0)
{
    for (auto& c : other)
    {
        c = max(c - lost_on_this, 0ll);
    }
}
source = std::move(target);
*/