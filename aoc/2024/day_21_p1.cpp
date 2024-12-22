#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

cPosition numPos(char c)
{
    switch (c)
    {
    case '7':
        return cPosition{ 0, 0 };
    case '8':
        return cPosition{ 0, 1 };
    case '9':
        return cPosition{ 0, 2 };

    case '4':
        return cPosition{ 1, 0 };
    case '5':
        return cPosition{ 1, 1 };
    case '6':
        return cPosition{ 1, 2 };

    case '1':
        return cPosition{ 2, 0 };
    case '2':
        return cPosition{ 2, 1 };
    case '3':
        return cPosition{ 2, 2 };

    case '0':
        return cPosition{ 3, 1 };
    case 'A':
        return cPosition{ 3, 2 };
    }
    P("PANIC: {}", c);
    return cPosition{ -1, -1 };
}

cPosition dirPos(char c)
{
    switch (c)
    {
    case '^':
        return cPosition(0, 1);
    case 'A':
        return cPosition(0, 2);

    case '<':
        return cPosition(1, 0);
    case 'v':
        return cPosition(1, 1);
    case '>':
        return cPosition(1, 2);
    }
    P("PANIC: {}", c);
    return cPosition{ -1, -1 };
}

// >V^<

string opsBetween(cPosition from, cPosition to)
{
    string res;
    for (int i = 0; i < to.col - from.col; ++i)
        res += '>';
    for (int i = 0; i < to.row - from.row; ++i)
        res += 'v';  
    for (int i = 0; i < from.col - to.col; ++i)
        res += '<';
    for (int i = 0; i < from.row - to.row; ++i)
        res += '^';
    return res;
}

bool isValidMove(cPosition pos, const string& move_s, int depth)
{
    for (auto c : move_s)
    {
        switch (c)
        {
        case '>':
            pos.col++;
            break;
        case '<':
            pos.col--;
            break;
        case 'v':
            pos.row++;
            break;
        case '^':
            pos.row--;
            break;
        }
        if (depth == 0)
        {
            if (pos.row == 3 && pos.col == 0)
                return false;
        }
        else
        {
            if (pos.row == 0 && pos.col == 0)
                return false;
        }
    }
    return true;
}

unordered_map<string, string> cache[26];

string calc(const string& txt, int depth)
{
    string& res = cache[depth][txt];
    if (!res.empty())
        return res;
    cPosition pos = depth == 0 ? numPos('A') : dirPos('A');
    for (auto c : txt)
    {
        cPosition nextPos = depth == 0 ? numPos(c) : dirPos(c);
        string move_s = opsBetween(pos, nextPos);
        if (depth < (is_first_part ? 2: 25))
        {
            string best;
            sort(ALL(move_s));
            do
            {
                if (!isValidMove(pos, move_s, depth))
                    continue;
                string s = move_s;
                s += 'A';
                string s1 = calc(s, depth + 1);
                if (best.empty() || s1.size() < best.size())
                {
                    best = s1;
                }
            } while (next_permutation(ALL(move_s)));
            res += best;
        }
        else
        {
            res += move_s;
            res += 'A';
        }
        pos = nextPos;
    }
    return res;
}

void solve(bool first)
{
    for (int i = 0; i < 26; ++i)
        cache[i].clear();
    ll res = 0;

    int expected[] = { 68, 60, 68, 64, 64 };

    for (auto& l : ls)
    {
        auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;

        string s = calc(l.txt, 0);

        int code = 0;
        code = (l.txt[0] - '0') * 100; 
        code += (l.txt[1] - '0') * 10;
        code += (l.txt[2] - '0') * 1;

        //P("{}: {}", l.txt, s);
        //P("code: {}, len: {}", code, s.size());
        if (is_example)
        {
            if (s.size() != expected[idx])
            {
                P("PANIC: expected: {}, got: {}", expected[idx], s.size());
                P("for {}: {}", l.txt, s);
            }
            else
            {
                P("GOOD");
            }
        }

        res += code * s.size();
    }

    RESULT(res);
}
