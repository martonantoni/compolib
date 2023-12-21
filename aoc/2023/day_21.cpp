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


    /*
    ll res = 0;
    cImage<char> img = loadImage(ls);
    cPosition sp;
    img.forAll([&](cPosition p) { if (img[p] == 'S') sp = p; });
    min_heap<pair<int, cPosition>> f;
    unordered_set<cPosition> r;
    unordered_set<ll> c;
    f.emplace(0, sp);
    while (!f.empty())
    {
        auto [s, p] = f.top();
        f.pop();
        if (s < 64)
        {
            img.forEach4Neighbour(p, [&](cPosition np)
                {
                    ll cc = (s << 24) | (np.col << 10) | np.row;
                    if (c.find(cc) == c.end())
                    {
                        c.emplace(cc);
                        if (img[np] == '.' || img[np]=='S')
                        {
                            //                        img[np] = 'V';
                            f.emplace(s + 1, np);
                        }
                    }
                });
        }
        else
        {
            r.insert(p);
        }
    }

    P("result: %lld", (ll)r.size());*/
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

volatile ll r2 = 0;

constexpr ll qn = 26'501'365;
//constexpr ll qn = 4;


void solveSecond()
{
    if (is_example)
         return;

    cImage<char> img = loadImage(ls);
//    vector<ll> qsteps = { 6ll, 10ll, 50ll, 100ll, 500ll, 1000ll, 5000ll, qn };
    cPosition sp;
    img.forAll([&](cPosition p) { if (img[p] == 'S') sp = p; });
    img[sp] = '.';
    P("img: %d x %d", img.h, img.w);
    P("start pos: %lld, %lld", sp.row, sp.col);

//    P("big grid: %lld x %lld = %lld", qn / img.h, qn / img.w, (qn * qn) / (img.h * img.w));


//    ll res = 0;

    int bh = qn / img.h;

    cPosition bp;

    ll res = 0;


    ll dist_up = sp.row + 1;
    ll dist_down = img.h - sp.row;
    ll dist_left = sp.col + 1;
    ll dist_right = img.w - sp.col;
    P("dist up: %lld, dist down: %lld", dist_left, dist_right);
    P("dist up: %lld, dist down: %lld", dist_up, dist_down);



    auto counts_bottom_right = getReachableCount(img, img.bottomRight());
    auto counts_bottom_left  = getReachableCount(img, img.bottomLeft());
    auto counts_top_right    = getReachableCount(img, img.topRight());
    auto counts_top_left     = getReachableCount(img, img.topLeft());

    auto counts_bottom_middle = getReachableCount(img, cPosition{ img.h - 1, sp.col });
    auto counts_top_middle = getReachableCount(img, cPosition{ 0, sp.col });
    auto counts_middle_left = getReachableCount(img, cPosition{ sp.row, 0 });
    auto counts_middle_right = getReachableCount(img, cPosition{ sp.row, img.w - 1 });

    res = reachable(getReachableCount(img, sp), qn);

    printf("target: %d\n", bh);

    loop_row(bp, make_pair(1, bh + 2))
    {
        if (bp.row % 5000 == 0)
        {
            printf("%lld\n", bp.row);
            fflush(stdout);
        }
        ll bottom_row_dist = (bp.row - 1) * img.h + dist_up;
        ll top_row_dist = (bp.row - 1) * img.h + dist_down;
        if (bottom_row_dist > qn && top_row_dist > qn)
            break;

        res += reachable(counts_bottom_middle, qn - bottom_row_dist);
        res += reachable(counts_top_middle, qn - top_row_dist);

        loop_col(bp, make_pair(1, bh + 2 ))
        {
            ll bottom_left_corner_dist = bottom_row_dist + (bp.col - 1) * img.h + dist_right;
            ll bottom_right_corner_dist = bottom_row_dist + (bp.col - 1) * img.h + dist_left;

            ll top_left_corner_dist = top_row_dist + (bp.col - 1) * img.h + dist_right;
            ll top_right_corner_dist = top_row_dist + (bp.col - 1) * img.h + dist_left;

            res += reachable(counts_bottom_right, qn - bottom_right_corner_dist);
            res += reachable(counts_bottom_left, qn - bottom_left_corner_dist);
            res += reachable(counts_top_right, qn - top_right_corner_dist);
            res += reachable(counts_top_left, qn - top_left_corner_dist);

        }
    }
    loop_col(bp, make_pair(1, bh +2))
    {
        ll left_col_dist = (bp.col - 1) * img.h + dist_right;
        ll right_col_dist = (bp.col - 1) * img.h + dist_left;

        res += reachable(counts_middle_left , qn - left_col_dist);
        res += reachable(counts_middle_right, qn - right_col_dist);

    }




    P("result: %lld", res);
}


 void solve(bool first) { first ? solveFirst() : solveSecond(); }

/*

 struct cCell
 {
     unique_ptr<cImage<char>> img;
     enum State { empty, inProgress, done } state = empty;
     ll target_count;
     ll count;
 };


 //    {
//        ll largest = qsteps.back();
//        cells.w = (largest * 2) / img.w + 2;
//        cells.h = (largest * 2) / img.h + 2;
//        P("required map: %lld x %lld = %lld", (ll)cells.w, (ll)cells.h, (ll)cells.w * (ll)cells.h);
////        cells.cells.resize(
//    }

 ll res = 0;


 auto [odd_target, event_target] = calculate2Dists(img);
 P("targets: %lld, %lld", odd_target, event_target);


 //    auto lpos = [&img](cPosition)->pair<cImage<char>, cPosition>;




 deque<pair<int, cPosition>> f;
 img.forEach4Neighbour(sp, [&](cPosition np) { if (img[np] == '.') f.emplace_back(1, np); });




 P("result: %lld", res);
 */