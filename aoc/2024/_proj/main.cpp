#include "aoc.h"

FILE* out = nullptr;

vector<cLine> ls;
vector<vector<cLine>> blocks;
bool is_first_part = true;
bool is_example = true;

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
        line.s.fromString(line.txt, main_delimeters, main_allow_empty_fields);    // <-----------------------------  delimeters
        line.i = line.s.toIntVector();
    }
    return lines;
}

const char* print_prefix = "";
vector<cLine> orig_example_lines, orig_lines;

void createBlocks()
{
    blocks.clear();
    blocks.emplace_back();
    for (auto& l : ls)
    {
        if (!l.is_empty)
            blocks.back().emplace_back(l);
        else
            blocks.emplace_back();
    }
}

void solvePart()
{
    const char* part_name = is_first_part ? "FIRST" : "SECOND";
    P("\n\n<<<<< {} PART >>>>>\n\n", part_name);
    if (!orig_example_lines.empty())
    {
        print_prefix = "EXAMPLE     ";
        is_example = true;
        printf("solving %s part... example\n", part_name);
        ls = orig_example_lines;
        createBlocks();
        solve(is_first_part);
        print_prefix = "";
        P("\n");
    }
    printf("solving %s part...\n", part_name);
    is_example = false;
    ls = orig_lines;
    createBlocks();
    {
        cLogPerformance_Guard perf(part_name);
        solve(is_first_part);
    }
    fflush(stdout); fflush(out);
}

int main()
{
    out = fopen(outFileName, "w");

   
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