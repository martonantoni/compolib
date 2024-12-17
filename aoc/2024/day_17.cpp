#include "aoc.h"
#include "bmp.h"
#include "utils.h"

#include <bitset>

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

// not: 2,5,6,4,1,2,0,5,7
     // 2,5,6,4,1,2,0,5,7

void printBin(ll x)
{
    P("{:x} ", x);
    for (int i = 32; i--;)
    {
        PC("{}", (x & (1ull << i)) ? 1 : 0);
    }
}

bool run(ll a, ll b, ll c, vector<ll>& prog)
{
    ll orig_a = a;

    ll ip = 0;

    int out_idx = 0;

    string res;

    auto operand = [&]() -> ll
        {
            auto o = prog[ip++];
            if (o <= 3) return o;
            if (o == 4) return a;
            if (o == 5) return b;
            if (o == 6) return c;
            return 0;
        };

    while (ip < prog.size())
    {
        switch (prog[ip++])
        {
        case 0: // ok
            a = a / (1 << operand());
            break;
        case 1:
            b = b ^ prog[ip++];
            break;
        case 2:
            b = operand() & 7;
            break;
        case 3: // ok
            if (a != 0)
            {
                ip = prog[ip];
            }
            else
            {
                ++ip;
            }
            break;
        case 4:
            ++ip;
            b = b ^ c;
            break;
        case 5: // ok
            //if (!res.empty())
            //    res += ",";
            //res += to_string(operand() & 7);
            if (out_idx == prog.size())
            {
                //if (out_idx >= 5)
                //{
                //    printBin(orig_a); PC("  ->  ");
                //    PC("died @ {}: too long", out_idx);
                //}
                return false;
            }
            if (auto o = (operand() & 7); o != prog[out_idx])
            {
                //if (out_idx >= 5)
                //{
                //    printBin(orig_a); PC("  ->  ");
                //    PC("died @ {}: expected {} got {}", out_idx, prog[out_idx], o);
                //}
                return false;
            }
            ++out_idx;
            break;
        case 6:
            b = a / (1 << operand());
            break;
        case 7:
            c = a / (1 << operand());
            break;
        }
    }
    if (out_idx != prog.size())
    {
        //if (out_idx >= 4)
        //{
        //    printBin(orig_a); PC("  ->  ");
        //    PC("died @ {}: too short", out_idx);
        //}
        return false;
    }
    return out_idx == prog.size();
}


// RESULT: 156668686
// That's not the right answer; your answer is too low.


void solve(bool first)
{
    if (is_example || !first)
        return;
//    ll res = 0;

    ll a = ls[0].i[2];
    ll b = ls[1].i[2];
    ll c = ls[2].i[2];

    vector<ll> prog = ls[4].i; prog.erase(prog.begin());

    // d 6ebd

    for (ll i = 0; i < 10000000000ll; ++i)
    {
 //       printBin(i); PC("  ->  ");
        if (run(0xd6ebdll | (i << 20ll) , b, c, prog))
        {
            RESULT(0xd6ebdll | (i << 20ll));
            fflush(out);
            break;
        }
    }


//    RESULT(res );
//    P("RESULT: {}", res);
}