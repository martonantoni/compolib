#include "utils.h"

cPattern::cPattern(const vector<ll>& nums)
    : nums(nums)
{
    auto i = find(nums.rbegin() + 1, nums.rend(), nums.back());
    if (i == nums.rend())
    {
        P("!!! Pattern not found !!!\n");
        return;
    }
    period = i - nums.rbegin();
    lead_length = i.base() - nums.begin();
}

ll cPattern::operator[](ll index0) const
{
    if (index0 < lead_length)
        return nums[index0];
    return nums[(index0 - lead_length) % period + lead_length];
}

void print_vll(const vector<ll>& nums)
{
    for (auto x : nums)
        P("%lld ", x);
}