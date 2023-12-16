#include "aoc.h"

struct cPattern
{
    ll lead_length = 0;
    ll period = 0;
    const vector<ll>& nums;

    cPattern(const vector<ll>& nums);
    ll operator[](ll index0) const;
};

void print_vll(const vector<ll>& nums);
