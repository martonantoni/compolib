#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

void solve(bool first)
{
    if (first)
        solveFirst();
    else
        solveSecond();
}

void solveFirst()
{
    int has2count = 0, has3count = 0;
    for (auto& l : ls)
    {
        vector<int> counts(26);
        for (auto c : l.txt)
            if (c >= 'a' && c <= 'z')
                counts[c - 'a']++;
        bool has2 = false, has3 = false;
        for (auto c : counts)
        {
            if (c == 2)
                has2 = true;
            if (c == 3)
                has3 = true;
        }
        if (has2)
            has2count++;
        if (has3)
            has3count++;
    }
    P("Result: {}\n", has2count * has3count);
}

void solveSecond()
{
    for (auto& l : ls)
    {
        for (auto& l2 : ls)
        {
            if (&l == &l2)
                continue;
            int diff = 0;
            for (int i = 0; i < l.txt.size(); ++i)
            {
                if (l.txt[i] != l2.txt[i])
                    diff++;
            }
            if (diff == 1)
            {
                P("Result: ");
                for (int i = 0; i < l.txt.size(); ++i)
                {
                    if (l.txt[i] == l2.txt[i])
                        PC("{}", l.txt[i]);
                }
                P("\n");
                return;
            }
        }
    }
}
