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
bool is_first_part = true;

cLogPerformance_Guard perf_guard("main");

vector<cLine> readFile(const char* fileName)
{
    cFastFileReader in(fileName);
    vector<cLine> lines;
    int idx = 0;
    for (auto file_line : in)
    {
        cLine& line = lines.emplace_back();
        line.txt = (string)file_line;
        line.idx = idx;
        ++idx;
        if (line.txt.empty())
        {
            line.is_empty = true;
            continue;
        }
        line.s.FromString(line.txt, main_delimeters, main_allow_empty_fields);    // <-----------------------------  delimeters
        line.i = line.s.ToIntVector();
    }
    return lines;
}

const char* print_prefix = "";
vector<cLine> orig_example_lines, orig_lines;

void solvePart()
{
    const char* part_name = is_first_part ? "FIRST" : "SECOND";
    P("\n\n<<<<< %s PART >>>>>\n\n", part_name);
    if (!orig_example_lines.empty())
    {
        print_prefix = "EXAMPLE     ";
        printf("solving %s part... example\n", part_name);
        ls = orig_example_lines;
        solve(is_first_part);
        print_prefix = "";
        P("\n");
    }
    printf("solving %s part...\n", part_name);
    ls = orig_lines;
    {
        cLogPerformance_Guard perf(part_name);
        solve(is_first_part);
    }
    fflush(stdout); fflush(out);
}

int main()
{
    out = fopen("aoc_out.txt", "w");

    printf("Reading example input...\n");
    orig_example_lines = readFile("aoc_example_in.txt");
    printf("reading input...\n");
    {
        cLogPerformance_Guard perf("input reading & parsing");
        orig_lines = readFile("aoc_in.txt");
        cFastFileReader in("aoc_in.txt");
    }
    is_first_part = true;
    solvePart();
    is_first_part = false;
    solvePart();

    fclose(out);
    out = nullptr;
    return 0;
}