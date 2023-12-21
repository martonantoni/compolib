#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

vector<ll> getReachableCount(cImage<char> img, cPosition entry)
{
    vector<ll> reachable;
    deque<pair<int, cPosition>> f;
    f.emplace_back(0, entry);
    ll last_steps = 0;
    while (!f.empty())
    {
        auto [s, p] = f.front();
        f.pop_front();
        if (s != last_steps)
        {
            reachable.emplace_back(count(ALL(img.cells), last_steps % 2 == 1 ? 'O' : 'E'));
            last_steps = s;
        }
        if (img[p] != '.')
            continue;
        img[p] = s % 2 == 1 ? 'O' : 'E';
        img.forEach4Neighbour(p, [&](auto np)
            {
                if (img[np] == '.')
                {
                    f.emplace_back(s + 1, np);
                }
            });
    }
    reachable.emplace_back(count(ALL(img.cells), last_steps % 2 == 1 ? 'O' : 'E'));
    return reachable;
}

ll reachable(const vector<ll>& counts, ll steps)
{
    if (steps < 0)
        return 0;
    ll cs = (ll)counts.size();
    if (steps < cs)
        return counts[steps];
    ll lci = cs - 1;
    return steps % 2 == lci % 2 ? counts[lci] : counts[lci - 1];
}

void solveFirst()
{
    cImage<char> img = loadImage(ls);
    cPosition sp;
    img.forAll([&](cPosition p) { if (img[p] == 'S') sp = p; });
    img[sp] = '.';
    auto counts = getReachableCount(img, sp);
    P("result: %lld", reachable(counts, is_example?6:64));
}

pair<ll,ll> calculate2Dists(cImage<char> img)
{
    deque<pair<int, cPosition>> f;
    f.emplace_back(1, cPosition { 0,0 });
    while (!f.empty())
    {
        auto [s, p] = f.front();
        f.pop_front();
        if (img[p] != '.')
            continue;
        img[p] = s % 2 == 1 ? 'O' : 'E';
        img.forEach4Neighbour(p, [&](auto np)
            {
                if (img[np] == '.')
                {
                    f.emplace_back(s + 1, np);
                }
            });
    }
    if (img.h < 20)
        printImage(img);

    return { count(ALL(img.cells), 'O'), count(ALL(img.cells), 'E') };
}

constexpr ll qn = 26'501'365;

void solveSecond()
{
    if (is_example)
         return;

    cImage<char> img = loadImage(ls);
    cPosition sp;
    img.forAll([&](cPosition p) { if (img[p] == 'S') sp = p; });
    img[sp] = '.';
    int bh = qn / img.h;

    cPosition bp;

    ll dist_up = sp.row + 1;
    ll dist_down = img.h - sp.row;
    ll dist_left = sp.col + 1;
    ll dist_right = img.w - sp.col;

    auto counts_bottom_right = getReachableCount(img, img.bottomRight());
    auto counts_bottom_left  = getReachableCount(img, img.bottomLeft());
    auto counts_top_right    = getReachableCount(img, img.topRight());
    auto counts_top_left     = getReachableCount(img, img.topLeft());

    auto counts_bottom_middle = getReachableCount(img, cPosition{ img.h - 1, sp.col });
    auto counts_top_middle = getReachableCount(img, cPosition{ 0, sp.col });
    auto counts_middle_left = getReachableCount(img, cPosition{ sp.row, 0 });
    auto counts_middle_right = getReachableCount(img, cPosition{ sp.row, img.w - 1 });

    ll res = reachable(getReachableCount(img, sp), qn);

    auto calculateCornerRow = [&](ll base_dist, const vector<ll>& counts)
        {
            ll remaining = qn - base_dist;
            ll sub_res = 0;
            ll cs = counts.size();
            ll start_col = remaining / img.h + 3;

            base_dist += (start_col - 1) * img.h;
            for (ll col = start_col; col >= 1; --col)
            {
                ll steps = qn - base_dist;
                if (steps >= cs)
                {
                    sub_res += reachable(counts, steps) * ((col + 1) / 2) + reachable(counts, steps + img.h) * (col / 2);
                    break;
                }
                sub_res += reachable(counts, steps);
                base_dist -= img.h;
            }
            return sub_res;
        };
    ll base_dist = 0;
    for(ll i = 1; i<bh+2;++i)
    {
        ll bottom_row_dist = base_dist + dist_up;
        ll top_row_dist = base_dist + dist_down;
        ll left_col_dist = base_dist + dist_right;
        ll right_col_dist = base_dist + dist_left;
        res += reachable(counts_middle_left, qn - left_col_dist);
        res += reachable(counts_middle_right, qn - right_col_dist);
        res += reachable(counts_bottom_middle, qn - bottom_row_dist);
        res += reachable(counts_top_middle, qn - top_row_dist);
        res += calculateCornerRow(bottom_row_dist + dist_right, counts_bottom_left);
        res += calculateCornerRow(bottom_row_dist + dist_left, counts_bottom_right);
        res += calculateCornerRow(top_row_dist + dist_right, counts_top_left);
        res += calculateCornerRow(top_row_dist + dist_right, counts_top_right);
        base_dist += img.h;
    }
    loop_col(bp, make_pair(1, bh +2))
    {
    }
    P("result: %lld", res);
    printf("+-----------\n| result diff: %lld\n+-----------\n", res - 600090522932119ll);
}


 void solve(bool first) { first ? solveFirst() : solveSecond(); }

