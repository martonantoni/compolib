#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

struct cNode
{
    vector<pair<cPosition, ll>> to; // where, len
    vector<pair<cPosition, ll>> to2;

    ll best = 0;
    bool visited = false;
    cPosition pos;
    bool exit = false;
};

cImage<cNode> nodes;

const string dir_marks = "^>v<"s;

ll calcLongest(cPosition from, ll len)
{
    ll best = len;
    for (auto [to, l] : nodes[from].to)
    {
        best = max(best, calcLongest(to, len + l));
    }
    return best;
}

ll calcLongest2(cPosition from, ll len)
{
    auto& node = nodes[from];
    if (node.exit)
        return len;
    if (node.visited)
        return 0;
    node.visited = true;
    ll best = 0;
    for (auto [to, l] : nodes[from].to2)
    {
        best = max(best, calcLongest2(to, len + l));
    }
    node.visited = false;
    return best;
}


void solveFirst()
{
    nodes.cells.clear();
    ll res = 0;

    cImage<char> img = loadImage(ls);
    nodes.initSize(img);
    img.forAll([&](cPosition p)
        {
            nodes[p].pos = p;
            if (img[p] != '.')
                return;
            int count = 0;
            img.forEach4Neighbour(p, [&](cPosition np)
                {
                    if (dir_marks.find(img[np]) != string::npos)
                    {
                        ++count;
                    }
                });
            if (count > 1)
                img[p] = 'x';
        });
    cPosition exit_pos = img.bottomRight();
    exit_pos += cPosition{ 0, -1 };
    vector<cPosition> f;
    f.emplace_back(0, 1);

    auto calcRoute = [&](cPosition pos, cPosition from) -> pair<cPosition, ll>
        {
            ll len = 1;
            for (;;)
            {
                if (img[pos] == 'x' || pos == exit_pos)
                    return make_pair(pos, len);
                for (auto dir : neighbour4Positions)
                {
                    auto np = pos + dir;
                    if (np == from || !img.isValidPos(np))
                        continue;
                    char c = img[np];
                    if (c != '#')
                    {
                        from = pos;
                        pos = np;
                        break;
                    }
                }
                ++len;
            }
        };

    while (!f.empty())
    {
        cPosition from = f.back();
        f.pop_back();
        if (from == exit_pos)
            continue;
        cNode& from_node = nodes[from];
        if (!from_node.to.empty())
            continue;
        for (auto [start_dir, mark] : views::zip(neighbour4Positions, dir_marks))
        {
            cPosition next_pos = from + start_dir;
            if (!img.isValidPos(next_pos))
                continue;
            if (char c = img[next_pos]; c == '.' || c == mark)
            {
                auto [dest, len] = calcRoute(next_pos, from);
                if (len > 0)
                {
                    from_node.to.emplace_back(dest, len);
                    f.emplace_back(dest);
                }
            }
        }
    }








    P("result: %lld", calcLongest({ 0, 1 }, 0));
}


void solveSecond()
{
    nodes.cells.clear();
    ll res = 0;

    cImage<char> img = loadImage(ls);
    nodes.initSize(img);
    img.forAll([&](cPosition p)
        {
            nodes[p].pos = p;
            if (img[p] != '.')
                return;
            int count = 0;
            img.forEach4Neighbour(p, [&](cPosition np)
                {
                    if (dir_marks.find(img[np]) != string::npos)
                    {
                        ++count;
                    }
                });
            if (count > 1)
                img[p] = 'x';
        });
    cPosition exit_pos = img.bottomRight();
    exit_pos += cPosition{ 0, -1 };
    nodes[exit_pos].exit = true;
    vector<cPosition> f;
    f.emplace_back(0, 1);

    auto calcRoute = [&](cPosition pos, cPosition from) -> pair<cPosition, ll>
        {
            ll len = 1;
            for (;;)
            {
                if (img[pos] == 'x' || pos == exit_pos)
                    return make_pair(pos, len);
                for (auto dir : neighbour4Positions)
                {
                    auto np = pos + dir;
                    if (np == from || !img.isValidPos(np))
                        continue;
                    char c = img[np];
                    if (c != '#')
                    {
                        from = pos;
                        pos = np;
                        break;
                    }
                }
                ++len;
            }
        };

    while (!f.empty())
    {
        cPosition from = f.back();
        f.pop_back();
        if (from == exit_pos)
            continue;
        cNode& from_node = nodes[from];
        if (!from_node.to.empty())
            continue;
        for (auto [start_dir, mark] : views::zip(neighbour4Positions, dir_marks))
        {
            cPosition next_pos = from + start_dir;
            if (!img.isValidPos(next_pos))
                continue;
            if (char c = img[next_pos]; c == '.' || c == mark)
            {
                auto [dest, len] = calcRoute(next_pos, from);
                if (len > 0)
                {
                    from_node.to.emplace_back(dest, len);
                    f.emplace_back(dest);
                }
            }
        }
    }


    for (auto& node : nodes.cells)
    {
        for (auto [to, len] : node.to)
        {
            node.to2.emplace_back(to, len);
            nodes[to].to2.emplace_back(node.pos, len);
        }
    }





    P("result 2: %lld", calcLongest2({ 0, 1 }, 0));
}


 void solve(bool first) { first ? solveFirst() : solveSecond(); }

