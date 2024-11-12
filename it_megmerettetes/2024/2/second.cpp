#include "aoc.h"
#include "bmp.h"
#include "utils.h"

/*
struct cLine
{
    vector<ll> i;
    cStringVector s;
    string txt;
    bool is_empty = false;
    int idx;
};
*/

const char* secondPartName = "kerek";

namespace
{

int left_of(const vector<int>& v, int i)
{
    int left_idx = i == 0 ? v.size() - 1 : i - 1;
    return v[left_idx];
}

int right_of(const vector<int>& v, int i)
{
    int right_idx = i == v.size() - 1 ? 0 : i + 1;
    return v[right_idx];
}

vector<int> removeMins(vector<int> v)
{
    vector<int> res;
    for(auto [i, x]: vs::enumerate(v))
    {
        if (x <= left_of(v, i) || x <= right_of(v, i))
        {
            //is_min[i] = true;
            res.push_back(x);
        }
    }
    return res;
}

vector<int> removeMaxs(vector<int> v)
{
    vector<int> res;
    for(auto [i, x]: vs::enumerate(v))
    {
        if (x >= left_of(v, i) || x >= right_of(v, i))
        {
            //is_min[i] = true;
            res.push_back(x);
        }
    }
    return res;
}


}

void solveSecond()
{
    vector<int> counts(100, 0);
    auto& is = ls[0].i;
    int sum = 0;

    for (auto x : is)
    {
        int r = x % 5;
        if (r == 4 || r == 1)
            ++sum;
    }
    P("sum: %d\n", sum);


    //for (auto i : is)
    //{
    //    counts[i]++;
    //    sum += i;
    //}
    //int num_seats = (int)is.size();
    //P("number of seats: %d\n", num_seats);
    //P("sum: %d\n", sum);

    //{
    //    vector<int> v;  // ll -> int
    //    for (auto x : is)
    //        v.push_back(x);

    //    for (;;)
    //    {
    //        int before = (int)v.size();
    //        v = removeMins(move(v));
    //        if (v.size() == before)
    //            break;
    //    }
    //    P("number of non-mins: %d\n", (int)v.size());
    //    P("v[0]: %d\n", v[0]);
    //    P("largest remaining: %d\n", *max_element(ALL(v)));
    //}

    //{
    //    vector<int> v;  // ll -> int
    //    for (auto x : is)
    //        v.push_back(x);

    //    for (;;)
    //    {
    //        int before = (int)v.size();
    //        v = removeMaxs(move(v));
    //        if (v.size() == before)
    //            break;
    //    }
    //    P("number of non-maxs: %d\n", (int)v.size());
    //    P("v[0]: %d\n", v[0]);
    //    P("smallest remaining: %d\n", *min_element(ALL(v)));
    //}


    //ll res = 0;
    //P("result: %lld", res);
}


