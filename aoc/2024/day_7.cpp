#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,:";
bool main_allow_empty_fields = false;


void solve(bool first)
{
    ll res = 0;
    vector<ll> merged_nums;
    for (auto& l : ls)
    {
        auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;

        ll target = l.i[0];

        auto nums = l.i | views::drop(1) | rng::to<vector<ll>>();

        ll opPosCount = nums.size() - 1;
        ll itarget = pow(3, opPosCount);
        for (ll i = 0; i < itarget; ++i)
        {
            ull sum = nums[0];
            for (ll j = 0, d = 1; j < opPosCount; ++j, d *= 3)
            {
                switch ((i / d) % 3)
                {
                case 0:
                    sum *= nums[j + 1];
                    break;
                case 1:
                    sum += nums[j + 1];
                    break;
                case 2:
                {
                    if (first)
                    {
                        sum = -1;
                    }
                    else
                    {
                        sum = stoll(to_string(sum) + to_string(nums[j + 1]));
                    }
                    break;
                }
                }
            }
            if (sum == target)
            {
                res += target;
                break;
            }
        }

    }
    P("Result: {}\n", res);
}
