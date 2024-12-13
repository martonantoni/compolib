#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

// 
//
//    When you are way too sleepy to think.......
//
//

void solve(bool first)
{
    ll res = 0;

    ll acost = 3, bcost = 1;

    for (auto&& [idx, block] : views::enumerate(blocks))
    {
        ll ax = stoll(block[0].s[2].substr(2));
        ll ay = stoll(block[0].s[3].substr(2));
        ll bx = stoll(block[1].s[2].substr(2));
        ll by = stoll(block[1].s[3].substr(2));

        ll tx = stoll(block[2].s[1].substr(2));
        ll ty = stoll(block[2].s[2].substr(2));
        if (!first)
        {
            tx += 10000000000000ll;
            ty += 10000000000000ll;
        }

        bool prefer_a = bx * 3 < ax || by * 3 < ay;

        if (prefer_a)
        {
            std::swap(ax, bx);
            std::swap(ay, by);
            acost = 1, bcost = 3;
        }
        else
        {
            acost = 3, bcost = 1;
        }


        ll best = std::numeric_limits<ll>::max();
        ll steps = 0;
        for (ll i = min(tx / bx, ty / by); i >= 0; --i)
        {
            if (++steps > 10000000)
                break;
            ll x = tx - i * bx;
            ll y = ty - i * by;

            ll xd = x / ax;
            ll yd = y / ay;
            if (abs(xd - yd) > 500000)
            {
                i -= abs(xd - yd) / max(ax, ay) - 1000;
                continue;
            }

            if (x % ax == 0 && y % ay == 0 && x / ax == y / ay)
            {
                ll cost = i * bcost + x / ax * acost;
                if(is_example)
                    P("block {} ... push B {} times, push A {} times --> cost: {}",idx , i, x / ax, cost);
                best = std::min(best, cost);
            }
        }
        if (best != std::numeric_limits<ll>::max())
            res += best;

    }


    RESULT(res);
}