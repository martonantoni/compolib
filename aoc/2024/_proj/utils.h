#include "aoc.h"

struct cIntMapper
{
    std::unordered_map<std::string, int> mMap;
    int operator[](const std::string& key)
    {
        auto it = mMap.find(key);
        if (it == mMap.end())
        {
            int idx = (int)mMap.size();
            mMap[key] = idx;
            return idx;
        }
        return it->second;
    }
};

struct cPattern
{
    ll lead_length = 0;
    ll period = 0;
    const vector<ll>& nums;

    cPattern(const vector<ll>& nums);
    ll operator[](ll index0) const;
};

void print_vll(const vector<ll>& nums);
