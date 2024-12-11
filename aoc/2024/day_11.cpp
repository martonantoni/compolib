#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

pair<ll, ll> splitStone(string& xs)
{
    auto left = xs.substr(0, xs.size() / 2);
    auto right = xs.substr(xs.size() / 2);
    while (right[0] == '0')
    {
        right = right.substr(1);
    }
    return { stoll(left), right.empty() ? 0 : stoll(right) };
}

void solve(bool first)
{
    ll res = 0;

    unordered_map<ll, ll> stones; // x, num of stones

    for (auto i : ls[0].i)
    {
        stones[i]++;
    }

    unordered_map<ll, ll> newStones;
    for (int step = 0; step < (first ? 25 : 75); ++step)
    {
        newStones.clear();
        for (auto&& [x, num] : stones)
        {
            if (x == 0)
            {
                newStones[1] += num;
            }
            else
            {
                auto cs = to_string(x);
                if (cs.size() % 2 == 0)
                {
                    auto [a, b] = splitStone(cs);
                    newStones[a] += num;
                    newStones[b] += num;
                }
                else
                {
                    newStones[x * 2024] += num;
                }
            }
        }
        swap(stones, newStones);
    }

    for (auto&& [x, num] : stones)
    {
        res += num;
    }

    RESULT(res);
}