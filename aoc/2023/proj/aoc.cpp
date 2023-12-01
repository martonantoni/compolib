#include "aoc.h"

const char* main_delimeters = " ,";

vector<const char*> nums { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "_", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine",  };

void solve(bool first)
{
    ll r = 0;
    for (auto& l : ls)
    {
        vector<size_t> left_pos, right_pos;
        for (auto& num : nums | views::take(first?10:20))
        {
            left_pos.emplace_back(l.txt.find(num));
            right_pos.emplace_back(l.txt.rfind(num));
        }
        auto left = rng::min_element(left_pos, less<>());
        auto right = rng::max_element(right_pos, less<>(), [](auto x) -> int { return x != string::npos ? (int)x : -1; });
        r += (left - left_pos.begin()) % 10 * 10;
        r += (right - right_pos.begin()) % 10;
    }
    P("%lld\n", r);
}

