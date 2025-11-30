#include "aoc.h"

#include <fstream>

FILE* out = nullptr;

vector<cLine> ls;
vector<vector<cLine>> blocks;
bool is_first_part = true;
bool is_example = true;

cLogPerformance_Guard perf_guard("main");

    //for (auto& l : ls)
    //{
    //    auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;
    //}


std::vector<cLine> readFile(const char* fileName) 
{
    std::ifstream in(fileName);
    if (!in) 
    {
        printf("Error opening file %s\n", fileName);
        return {};
    }

    std::vector<cLine> lines;
    std::string l;
    int idx = 0;

    while (std::getline(in, l)) 
    {
        cLine& line = lines.emplace_back();
        line.txt = l;
        line.idx = idx++;
        if (l.empty()) {
            line.is_empty = true;
            continue;
        }
        line.s.fromString(l, main_delimeters, main_allow_empty_fields);
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
    perf_guard.waypoint("main entered");
    out = fopen(outFileName, "w");
    perf_guard.waypoint("output file opened");

   
    printf("Reading example input...\n");
    orig_example_lines = readFile("aoc_example_in.txt");
    perf_guard.waypoint("example input read");
    printf("reading input...\n");
    perf_guard.waypoint("input read");
    {
        cLogPerformance_Guard perf("input reading & parsing");
        orig_lines = readFile("aoc_in.txt");
    }
    perf_guard.waypoint("parse done");
    is_first_part = true;
    solvePart();
    perf_guard.waypoint("first part solved");
    is_first_part = false;
    solvePart();
    perf_guard.waypoint("second part solved");

    fclose(out);
    out = nullptr;
    return 0;
}