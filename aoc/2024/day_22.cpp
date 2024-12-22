#include "aoc.h"
#include "bmp.h"
#include "utils.h"

#include <future>

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

constexpr ll mod = 16777216ll;

char buf[2001];
char buf2[2001];

void solve(bool first)
{
    ll res = 0;

    vector<string> nums;
    vector<string> prices;

    for (auto& l : ls)
    {
        auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;

        ll x = l.i[0];
        ll prev = x % 10;
        for (int i = 0; i < 2000; ++i)
        {
            auto y = x * 64ll;
            x = x ^ y;
            x %= mod;

            y = x / 32ll;
            x = x ^ y;
            x %= mod;

            y = x * 2048ll;
            x = x ^ y;
            x %= mod;

            ll diff = x % 10 - prev;
            buf[i] = 'm' + diff;
            buf2[i] = 'a' + x % 10;
            prev = x % 10;
        }
        if (first)
            res += x;
        buf[2000] = 0;
        buf2[2000] = 0;
        nums.emplace_back(buf);
        prices.emplace_back(buf2);
    }
    if (first)
    {
        RESULT(res);
        return;
    }
    buf[4] = 0;
    res = 0;

    auto calc = [&](char c0)
        {
            char b[5];
            b[4] = 0;
            b[0] = c0;
            ll best = 0;
            for (char c1 = 'm' - 10; c1 <= 'm' + 10; ++c1)
            {
                b[1] = c1;
                for (char c2 = 'm' - 10; c2 <= 'm' + 10; ++c2)
                {
                    b[2] = c2;
                    for (char c3 = 'm' - 10; c3 <= 'm' + 10; ++c3)
                    {
                        b[3] = c3;
                        ll count = 0;
                        for (auto&& [s, p] : views::zip(nums, prices))
                        {
                            auto i = s.find(b);
                            if (i == string::npos)
                                continue;
                            count += p[i + 3] - 'a';
                        }
                        best = max(best, count);
                    }
                }
            }
            return best;
        };


    vector<future<ll>> fs;

    for (char c0 = 'm' - 10; c0 <= 'm' + 10; ++c0)
    {
        auto f = async(launch::async, calc, c0);
        fs.emplace_back(std::move(f));
    }

    for (auto& f : fs)
    {
        res = max(res, f.get());
    }

    RESULT(res);
}
