#include "aoc.h"
#include "bmp.h"
#include "utils.h"

#include <bitset>

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,-";
bool main_allow_empty_fields = false;

struct cNode
{
    bitset<4000> hasConnection;
    string name;
};

vector<cNode> nodes;

void solve(bool first)
{
    if (is_example)
        return;

    ll res = 0;

    nodes.clear();
    nodes.resize(ls.size());
    unordered_map<string, int> nodeIDs;

    auto id2idx = [&](const string& id) -> int
        {
            auto it = nodeIDs.find(id);
            if (it == nodeIDs.end())
            {
                int idx = (int)nodeIDs.size();
                nodeIDs[id] = idx;
                nodes[idx].name = id;
                return idx;
            }
            return it->second;
        };

    for (auto& l : ls)
    {
        auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;

        auto idx1 = id2idx(l.s[0]);
        auto idx2 = id2idx(l.s[1]);

        nodes[idx1].hasConnection[idx2] = true;
        nodes[idx2].hasConnection[idx1] = true;
    }
    nodes.resize(nodeIDs.size());

    vector<vector<int>> groups;
    for (int i = 0; i < nodes.size(); ++i)
    {
        groups.push_back({ i });
    }
    vector<vector<int>> new_groups;
    for (;;)
    {
        new_groups.clear();
        for (auto& g : groups)
        {
            for (int i = g.back() + 1; i < nodes.size(); ++i)
            {
                bool all_connected = true;
                for (auto j : g)
                {
                    if (!nodes[j].hasConnection[i])
                    {
                        all_connected = false;
                        break;
                    }
                }
                if (all_connected)
                {
                    auto new_g = g;
                    new_g.push_back(i);
                    new_groups.push_back(new_g);
                }
            }
        }
        if (new_groups.size() == 1)
        {
            vector<string> ids;
            for (auto idx : new_groups[0])
            {
                ids.push_back(nodes[idx].name);
            }
            sort(ALL(ids));
            P("ID:");
            for (auto& id : ids)
            {
                PC("{},", id);
            }
            return;
        }
        else if (first && new_groups[0].size() == 3)
        {
            for (auto& g : new_groups)
            {
                bool hasT = false;
                for (auto gi : g)
                {
                    if(nodes[gi].name[0] == 't')
                    {
                        hasT = true;
                        break;
                    }
                }
                if (hasT)
                    ++res;
            }
            RESULT(res);
            return;
        }
        groups = std::move(new_groups);
    }
}
