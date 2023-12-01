#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>

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
#include <unordered_set>
#include <cstddef>
#include <cinttypes>
#include <regex>
#include <filesystem>
#include <ctime>

#include "FastFileReader.h"

namespace fs = std::filesystem;

#undef max
#undef min
#undef I

#ifdef __GNUC__ 
#pragma GCC diagnostic ignored "-Wunused-result"
#endif

FILE* out = nullptr;

#ifdef _DEBUG
#define ASSERT(x) if(!(x)) __debugbreak()
#define P(...) if(out) fprintf(out, __VA_ARGS__)
#define D(expression) expression
#else
char* crash_please = (char*)42;
#define ASSERT(x) 
#define P(...) if(out) fprintf(out, __VA_ARGS__)
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
        P("\n--- Elapsed %llu.%llu ms in %s ---\n", (unsigned long long)Elapsed.count() / 1000, (unsigned long long)Elapsed.count() % 1000, mName);
    }
    void waypoint()
    {
        auto EndTime = std::chrono::high_resolution_clock::now();
        auto Elapsed = std::chrono::duration_cast<std::chrono::microseconds>(EndTime - mStartTime);
        P("+ %llu.%llu ms: ", (unsigned long long)Elapsed.count() / 1000, (unsigned long long)Elapsed.count() % 1000);
    }
} perf_guard("main");



using namespace std;
using namespace std::chrono;
using namespace std::string_literals;
using ull = unsigned long long;
using ll = long long;
using cIntVector = vector<ll>;

#define ALL(cont) cont.begin(), cont.end()
#define FOR(var, max_value) for(decltype(max_value) var=0;var<max_value;++var)

constexpr ll mod = 1'000'000'007;

class cStringVector : public std::vector<std::string> // could be in its separate header file, but I don't care.
{
public:
    cStringVector() {}
    cStringVector(const std::string& SourceString, const std::string& Delimeters, bool EmptyFieldsAllowed = true);
    void FromString(const std::string& SourceString, const std::string& Delimeters, bool EmptyFieldsAllowed = true);
    int FindIndex(const std::string& Token, int From = 0) const; // returns -1 if not found
    cIntVector ToIntVector() const;
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
        IntVector[i] = atol((*this)[i].c_str());
    return IntVector;
}

struct cLine
{
    vector<ll> i;
    cStringVector s;
    string txt;
    bool is_empty = false;
};

vector<cLine> ls;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* ns[9] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

void solve()
{
    ll r = 0;
    for (auto& l : ls)
    {
        auto i = find_if(l.txt.begin(), l.txt.end(), [](auto c) { return c >= '0' && c <= '9'; });
        int first = *i - '0';
        int x = 1;
        size_t best = i - l.txt.begin();
        for (auto& t : ns)
        {
            auto k = l.txt.find(t);
            if (k!=string::npos && k < best)
            {
                first = x;
                best = k;
                i = l.txt.begin();
            }
            ++x;
        }
        auto j = i;
        auto lj = i;
        int last = -1;
        while (j != l.txt.end())
        {
            lj = j;
            j = find_if(j+1, l.txt.end(), [](auto c) { return c >= '0' && c <= '9'; });
            if (j != l.txt.end())
                last = *j - '0';
        }

        x = 1;
        best = lj - l.txt.begin();
        for (auto& t : ns)
        {
            auto k = l.txt.rfind(t);
            if (k != string::npos)
            {
                if (k > best)
                {
                    last = x;
                    best = k;
                }
            }
            ++x;
        }



        if (last == -1)
            last = first;

        P("%d\n", first * 10 + last);

        r += first * 10 + last;
    }
    P("%lld", r);
} 



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int main()
{
    out = fopen("aoc_out.txt", "w");

    cFastFileReader in("aoc_in.txt");
    for (auto file_line : in)
    {
        cLine& line = ls.emplace_back();
        line.txt = (string)file_line;
        if (line.txt.empty())
        {
            line.is_empty = true;
            continue;
        }
        line.s.FromString(line.txt, " ,");    // <-----------------------------  delimeters
        line.i = line.s.ToIntVector();
    }

    solve();

    fclose(out);
    out = nullptr;
    return 0;
}