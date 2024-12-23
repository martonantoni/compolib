#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;


//        POS:
// +-----+-----+-----+     +-----+-----+-----+
// |  0  |  1  |  2  |     |  X  |  ^  |  A  |
// +-----+-----+-----+     +-----+-----+-----+
// |  3  |  4  |  5  |     |  <  |  v  |  >  |
// +-----+-----+-----+     +-----+-----+-----+


vector<ll> cache;
int max_level = 0;

ll calcSequence(int horiz_steps, int vert_steps, bool horiz_first_valid, bool vert_first_valid, int level);

ll stepsRequiredWithPush(int from, int to, int level)
{
    if (level == 0)
        return 1;
    if (from == to)
        return 1; // the push
    ll& res = cache[level * 6 * 6 + from * 6 + to];
    if (res != 0)
        return res;

    int from_row = from / 3;
    int from_col = from % 3;
    int to_row = to / 3;
    int to_col = to % 3;

    int horiz_steps = to_col - from_col;
    int vert_steps = to_row - from_row;
    bool horiz_first_valid = to != 3 || from_row != 0;
    bool vert_first_valid = from != 3 || to_row != 0;

    res = calcSequence(horiz_steps, vert_steps, horiz_first_valid, vert_first_valid, level);
    return res;
}

ll calcSequence(int horiz_steps, int vert_steps, bool horiz_first_valid, bool vert_first_valid, int level)
{
    ll res = numeric_limits<ll>::max();
    array<int, 10> seq;
    int seq_len = 0;

    auto calcSequence = [&]()->ll
        {
            ll cost = 0;
            ll pos = 2;
            for (int i = 0; i < seq_len; ++i)
            {
                int next = seq[i];
                cost += stepsRequiredWithPush(pos, next, level - 1);
                pos = next;
            }
            cost += stepsRequiredWithPush(pos, 2, level - 1);
            return cost;
        };

    while(horiz_steps>0)
    {
        seq[seq_len++] = 5;
        horiz_steps--;
    }
    while(horiz_steps<0)
    {
        seq[seq_len++] = 3;
        horiz_steps++;
    }
    while (vert_steps > 0)
    {
        seq[seq_len++] = 4;
        vert_steps--;
    }
    while (vert_steps < 0)
    {
        seq[seq_len++] = 1;
        vert_steps++;
    }
    if (horiz_first_valid)
    {
        res = calcSequence();
    }
    reverse(begin(seq), begin(seq) + seq_len);
    if (vert_first_valid)
    {
        res = min(res, calcSequence());
    }
    return res;
}

// +-----+-----+-----+     +-----+-----+-----+
// |  7  |  8  |  9  |     |  0  |  1  |  2  |
// +-----+-----+-----+     +-----+-----+-----+
// |  4  |  5  |  6  |     |  3  |  4  |  5  |
// +-----+-----+-----+     +-----+-----+-----+
// |  1  |  2  |  3  |     |  6  |  7  |  8  |
// +-----+-----+-----+     +-----+-----+-----+
// |  X  |  0  |  A  |     |  9  | 10  |  11 |
// +-----+-----+-----+     +-----+-----+-----+

ll stepsReqOnNumpad(char from_c, char to_c)
{
    int from = "789456123X0A"s.find(from_c);
    int to = "789456123X0A"s.find(to_c);

    int from_row = from / 3;
    int from_col = from % 3;
    int to_row = to / 3;
    int to_col = to % 3;
    int horiz_steps = to_col - from_col;
    int vert_steps = to_row - from_row;
    

    bool horiz_first_valid = from_row != 3 || to_col != 0;
    bool vert_first_valid = from_col != 0 || to_row != 3;

    return calcSequence(horiz_steps, vert_steps, horiz_first_valid, vert_first_valid, max_level);
}

void solve(bool first)
{

    max_level = first ? 3 : 26;

    cache.clear();
    cache.resize(30 * 6 * 6, 0);

    ll res = 0;

    for (auto& l : ls)
    {
        auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;

        ll steps = stepsReqOnNumpad('A', txt[0]);
        steps += stepsReqOnNumpad(txt[0], txt[1]);
        steps += stepsReqOnNumpad(txt[1], txt[2]);
        steps += stepsReqOnNumpad(txt[2], 'A');

        ll code = 0;
        code = (l.txt[0] - '0') * 100;
        code += (l.txt[1] - '0') * 10;
        code += (l.txt[2] - '0') * 1;
        
        P("{}: steps {}, code: {}, code * steps: {}", txt, steps, code, code * steps);


        res += code * steps;
    }

    RESULT(res);
}
