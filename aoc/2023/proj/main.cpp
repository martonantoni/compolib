#include "aoc.h"

FILE* out = nullptr;

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
    {
        IntVector[i] = atoll((*this)[i].c_str());
    }
    return IntVector;
}



vector<cLine> ls;

cLogPerformance_Guard perf_guard("main");

int main()
{
    out = fopen("aoc_out.txt", "w");

//     printf("reading input...\n");
//     {
//         cLogPerformance_Guard perf("input reading & parsing");
//         cFastFileReader in("aoc_in.txt");
//         int idx = 0;
//         for (auto file_line : in)
//         {
//             cLine& line = ls.emplace_back();
//             line.txt = (string)file_line;
//             line.idx = idx;
//             ++idx;
//             if (line.txt.empty())
//             {
//                 line.is_empty = true;
//                 continue;
//             }
//             line.s.FromString(line.txt, main_delimeters);    // <-----------------------------  delimeters
//             line.i = line.s.ToIntVector();
//         }
//     }
//     auto orig_lines = ls;

    P("<<<<< FIRST PART >>>>>\n\n");
    printf("solving first part...\n");
    {
        cLogPerformance_Guard perf("first part");
        solve(true);
    }
//     fflush(stdout); fflush(out);
    P("\n\n<<<<< SECOND PART >>>>>\n\n");
//     ls = orig_lines;
    printf("solving second part...\n");
    {
        cLogPerformance_Guard perf("second part");
        solve(false);
    }

    fclose(out);
    out = nullptr;
    return 0;
}