#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* main_delimeters = " ,@";
bool main_allow_empty_fields = false;


struct Point {
    double x, y;
};

struct Vector {
    double dx, dy;
};

// Function to find the intersection point of two lines
std::tuple<bool, Point> findIntersection(const Point& P1, const Vector& V1,
    const Point& P2, const Vector& V2) {
    double det = V1.dx * V2.dy - V1.dy * V2.dx;

    // Check if the lines are parallel (no intersection)
    if (std::abs(det) < 1e-10) {
        return std::make_tuple(false, Point{ 0.0, 0.0 });
    }

    // Calculate parameters t and s for the intersection point
    double t = ((P2.x - P1.x) * V2.dy - (P2.y - P1.y) * V2.dx) / det;
    double s = ((P2.x - P1.x) * V1.dy - (P2.y - P1.y) * V1.dx) / det;

    // Check if the intersection point is within the line segments and in the direction of the vectors
    if (t >= 0.0 && s >= 0.0) {
        Point intersectionPoint{
            P1.x + t * V1.dx,
            P1.y + t * V1.dy
        };
        return std::make_tuple(true, intersectionPoint);
    }
    else {
        // Lines do not intersect within the line segments or not in the direction of vectors
        return std::make_tuple(false, Point{ 0.0, 0.0 });
    }
}

void solveFirst()
{
    ll res = 0;

    double cmin = is_example ? 7.0 :  200000000000000.0;
    double cmax = is_example ? 27.0 : 400000000000000.0;

    vector<pair<Point, Vector>> hs;

    for (auto& l : ls)
    {
        hs.emplace_back(Point{ (double)l.i[0],(double)l.i[1] }, Vector{ (double)l.i[3], (double)l.i[4] });
    }
    FOR(i, ls.size())
    {
        for (size_t j = i + 1; j < ls.size(); ++j)
        {
            auto [has, p] = findIntersection(hs[i].first, hs[i].second, hs[j].first, hs[j].second);
            if (has)
            {
                if (p.x >= cmin && p.x <= cmax && p.y >= cmin && p.y <= cmax)
                {
                    double dxmin = p.x - cmin;
                    double dxmax = cmax - p.x;
                    double dymin = p.y - cmin;
                    double dymax = cmax - p.y;
                    if(dxmin<10000.0 || dxmax<10000.0 || dymin < 10000.0 || dymax < 10000.0)
                        P("%lf, %lf     min_diff: %lf, %lf      max_diff: %lf, %lf", p.x, p.y, p.x-cmin, p.y-cmin, cmax-p.x, cmax-p.y);
                    ++res;
                }
            }
        }
    }


    P("result: %lld", res);
}



using cVec = array<ll, 3>;
using Vec3 = cVec;

cVec operator-(const cVec& l, const cVec& r)
{
    return cVec{ l[0] - r[0], l[1] - r[1], l[2] - r[2] };
}

struct cHailstone
{
    cVec v;
    cVec c;

    cVec after(ll t)
    {
        return { c[0] + v[0] * t, c[1] + v[1] * t, c[2] + v[2] * t };
    }
    auto operator<=>(const cHailstone&) const = default;
};

// 653583681465915 too low

vector<cHailstone> hs;

ll best = 0;
/*
void simulate(ll fs, ll ss, ll d1, ll d2)
{
    auto second_after_d = hs[ss].after(d2);
    auto first_after_d = hs[fs].after(d1);
    cVec v = second_after_d - first_after_d;
    FOR(j, 3)
        v[j] /= d2 - d1;

    ll bad = 0;

    for (ll i = 0; i < (ll)hs.size(); ++i)
    {
        if (i == fs || i == ss)
            continue;
        auto& target = hs[i];
        auto target_v = target.v;
        auto target_p = target.after(d1);
        double t[3];
        FOR(j, 3)
        {
            t[j] = first_after_d[j] == target_p[j] ? 0.0:
                static_cast<double>(first_after_d[j] - target_p[j]) /
                static_cast<double>(target_v[j] - v[j]);
        }
        if (t[0] == t[1] && t[1] == t[2] && t[0]>0 && !isinf(t[0]))
            continue;

        if (t[0] == 0.0 && target_v[0] == v[0] && t[1] == t[2] && t[1]>0 && !isinf(t[1]))
            continue;

        if (t[1] == 0.0 && target_v[1] == v[1] && t[0] == t[2] && t[2] > 0 && !isinf(t[2]))
            continue;

        if (t[2] == 0.0 && target_v[2] == v[2] && t[1] == t[0] && t[0] > 0 && !isinf(t[1]))
            continue;

        if (t[0] == 0.0 && t[1] == 0.0 && target_v[0] == v[0] && target_v[1] == v[1] &&
            !isinf(t[2]) && t[2] > 0)
            continue;
        if (t[0] == 0.0 && t[2] == 0.0 && target_v[0] == v[0] && target_v[2] == v[2] &&
            !isinf(t[1]) && t[1] > 0)
            continue;
        if (t[2] == 0.0 && t[1] == 0.0 && target_v[2] == v[2] && target_v[1] == v[1] &&
            !isinf(t[1]) && t[1] > 0)
            continue;

        ++bad;
        if (bad > best)
            return;
    }
//    P("HIT... fs: %lld, ss: %lld, d1: %lld, d2: %lld, i: %lld",
  //      fs, ss, d1, d2, -1ll);

    if (bad >= best)
        return;
    best = bad;
    cHailstone stone;
    stone.c = hs[fs].after(d1);
    stone.v = v;

    cVec stone_start = stone.after(-d1);

    P("BAD: %lld", bad);
    P("stone start: %lld %lld %lld", (ll)stone_start[0], (ll)stone_start[1], (ll)stone_start[2]);
    P("result: %lld", (ll)stone_start[0] + (ll)stone_start[1] + (ll)stone_start[2]);
    fflush(out);
}*/

bool check(ll first_i, ll second_i, ll vx, ll vy)
{
    auto& first = hs[first_i];
    auto& second = hs[second_i];

    ll a = first.v[0] - vx;
    ll b = vx - second.v[0];
    ll c = first.c[0] - second.c[0];

    ll d = first.v[1] - vy;
    ll e = vy - second.v[1];
    ll f = first.c[1] - second.c[1];

    ll top = c * d - f * a;
    ll bottom = e * a - b * d;
    if (bottom == 0)
    {
        //P("bottom 0");
        //P("first: %lld, second: %lld, vx: %lld, vy: %lld", first_i, second_i, vx, vy);
        return false;
    } 
    if (top % bottom == 0)
    {
        ll t1 = top / bottom;
        if (t1 < 0)
            return false;
        //ll x_t1 = second.c[0]+second.v[0]
        //for (ll i = 0; i < (ll)hs.size(); ++i)
        //{
        //    if (i == first_i || i == second_i)
        //        continue;
        //    ll t = 
        //}
        if (t1 > 0)
        {
            //P("*** HIT ***");
            //P("first: %lld, second: %lld, vx: %lld, vy: %lld", first_i, second_i, vx, vy);
            //P("t1: %lld", top / bottom);
//            fflush(out);
            return true;
        }
    }
    return false;
}

pair<ll, ll> calculate(ll first_i, ll second_i, ll vx, ll vy)
{
    auto& first = hs[first_i];
    auto& second = hs[second_i];

    ll a = first.v[0] - vx;
    ll b = vx - second.v[0];
    ll c = first.c[0] - second.c[0];

    ll d = first.v[1] - vy;
    ll e = vy - second.v[1];
    ll f = first.c[1] - second.c[1];

    ll top = c * d - f * a;
    ll bottom = e * a - b * d;


    ll check_t1 = top % bottom;
    ll t1 = top / bottom;

    return { t1, check_t1 };
}

void solveSecond()
{
    fflush(out);
    ll res = 0;

    hs.clear();
    for (auto& l : ls)
    {
        hs.emplace_back();
        FOR(i, 3)
        {
            hs.back().c[i] = l.i[i];
            hs.back().v[i] = l.i[i + 3];
        }
    }
    ll hss = (ll)hs.size();
    best = hss;
    if (is_example)
        return;

    ll vx = -65, vy = -86;

    vector<ll> t({
        950225482094ll,
        584973040098ll,
        324090870134ll,
        337753745269ll });

    ll z0 = hs[0].c[2] + hs[0].v[2] * t[0];
    ll z1 = hs[1].c[2] + hs[1].v[2] * t[1];

    ll dt = t[1] - t[0];
    ll dz = z1 - z0;

    ll vz_check = dz % dt;
    ll vz = dz / dt;

    ll x0 = hs[0].c[0] + hs[0].v[0] * t[0];
    ll y0 = hs[0].c[1] + hs[0].v[1] * t[0];

    ll x = x0 - vx * t[0];
    ll y = y0 - vy * t[0];
    ll z = z0 - vz * t[0];

    P("%lld %lld %lld  ---- result ---> %lld", x, y, z, x + y + z);

    return;

//    calculate(0, 1, vx, vy);
    for (ll first = 0; first < hss; ++first)
    {
        for (ll second = 0; second < hss; ++second)
        {
            if (first == second)
                continue;
            auto [t1, ct1] = calculate(first, second, vx, vy);
            P("%lld: %lld (%lld)", second, t1, ct1);
        }
    }
    return;


//    for (ll vx = -1000; vx < 1000; ++vx)
//    {
        printf("%lld\n", vx);
//        for (ll vy = -1000; vy < 1000; ++vy)
//        {
            ll hit = 0;
            for (ll first = 0; first < hss; ++first)
            {
                for (ll second = 0; second < hss; ++second)
                {
                    if (first == second)
                        continue;
                    if (check(first, second, vx, vy))
                        ++hit;
                }
            }
            if(hit > 100)
                P("%lld, %lld ----> %lld", vx, vy, hit);
//        }
//    }

    //sort(ALL(hs));
    //for (auto& h : hs)
    //{
    //    P("%lf %lf %lf", h.v[0], h.v[1], h.v[2]);
    //}
    //return;

    //FOR(i, hss)
    //{
    //    auto& ih = hs[i];
    //    for (ll j = i + 1; j < hss; ++j)
    //    {
    //        auto& jh = hs[j];
    //        if (abs(ih.v[0] / jh.v[0]) == abs(ih.v[1] / jh.v[1]) && abs(ih.v[1] / jh.v[1]) == abs(ih.v[2] / jh.v[2]))
    //        {
    //            P("%lld and %lld are parallel", i, j);
    //            P("%lf %lf %lf  |  %lf %lf %lf", ih.v[0], ih.v[1], ih.v[2], jh.v[0], jh.v[1], jh.v[2]);
    //        }
    //    }
    //}
    //return;


//    if (is_example)
  //  {
    //    simulate(4, 1, 1, 3);
      //  return;
    //}



    //for (ll d1 = 1ll; d1 < 100ll; ++d1)
    //{
    //    printf("d1: %lld\n", d1); 
    //    fflush(stdout);
    //    for (ll d2 = d1 + 1; d2 < 100000000ll; ++d2)
    //    {
    //        for (ll first_stone = 0; first_stone < hss; ++first_stone)
    //        {
    //            for (ll second_stone = 0; second_stone < hss; ++second_stone)
    //            {
    //                if (first_stone == second_stone)
    //                    continue;
    //                simulate(first_stone, second_stone, d1, d2);
    //            }
    //        }
    //    }
    //}



    P("result: %lld", res);
}


 void solve(bool first) { first ? solveFirst() : solveSecond(); }

