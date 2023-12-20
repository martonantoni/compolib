#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* main_delimeters = " ,->";
bool main_allow_empty_fields = false;

ll gcd(ll a, ll b)
{
    while (b) b ^= a ^= b ^= a %= b;
    return a;
}

ll lcm(ll a, ll b)
{
    return (a / gcd(a, b)) * b;
}

ll lcm(const vector<ll>& nums)
{
    if (nums.empty())
        return 0;
    if (nums.size() == 1)
        return nums[0];
    ll res = lcm(nums[0], nums[1]);
    for (size_t i = 2; i < nums.size(); ++i)
        res = lcm(res, nums[i]);
    return res;
}


struct cNode
{
    vector<pair<ll,ll>> to;
    enum Type { broadcaster, flip_flop, conjunction } type;
    bool is_on = false;
    vector<char> input_mem;
    string name;
    ll node_idx;

    bool is_interesting = false;
};

ll low_count = 0, high_count = 0;
vector<cNode> nodes;
unordered_map<string, ll> names;


vector<string> last_state;

void printState(ll button)
{
    vector<string> state;
    state.reserve(nodes.size());
    for (auto& node : nodes)
    {
        if (!node.is_interesting)
            continue;
        if (node.type == cNode::flip_flop)
        {
            continue;
//            state_line += node.is_on ? "ON" : "OFF";
        }
        string state_line = node.name + " : ";
        if (node.type == cNode::conjunction)
        {
            for (auto s : node.input_mem)
                state_line += s;
        }
        state.emplace_back(state_line);
    }
    for (const auto& [current, prev] : vs::zip(state, last_state))
    {
        if (current != prev)
        {
            P("%lld: %s", button, current.c_str());
            //printf("%lld: %s\n", button, current.c_str());
            //fflush(stdout);
            //fflush(out);
        }
    }
    swap(state, last_state);
}

int bs[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

void printConjuctionNode(ll button, cNode& node)
{
    string line = node.name + " : ";
    ll value = 0;
    if (node.type == cNode::conjunction)
    {
        for (auto [idx, s] : vs::enumerate(node.input_mem))
        {
            line += s;
            if(s=='H')
                value |= 1ll << (bs[idx]);
        }
    }
    P("%lld  %s  %lld", button, line.c_str(), value);
}

void printAllH(ll button)
{
    for (auto& node : nodes)
    {
        if (!node.is_interesting)
            continue;
        if (node.type == cNode::conjunction && node.input_mem.size() > 1)
        {
            //if (find(ALL(node.input_mem),'L') == node.input_mem.end())
            //{
            //    P("%lld : %s all H", button, node.name.c_str());
            //    printf("%lld : %s all H", button, node.name.c_str());
            //    fflush(out);
            //    fflush(stdout);
            //}
        }
    }

}

bool gotcha = false;
deque<tuple<ll, ll, char>> ss;
ll broadcaster;

ll checked_node_idx[4];
ll last_all_h[4] = { 0, 0, 0, 0 };

void simulate(ll button)
{

    auto sendSignal = [&](cNode& node, char signal)
        {
            for (auto [to, in_idx] : node.to)
            {
//                P("%s --%c--> %s", node.name.c_str(), signal, nodes[to].name.c_str());
                //if (nodes[to].type == cNode::flip_flop && signal == 'H')
                //    continue;
                ss.emplace_back(to, in_idx, signal);
            }
            if (signal == 'L')
                low_count += (ll)node.to.size();
            else
                high_count += (ll)node.to.size();
        };

    ++low_count;
    sendSignal(nodes[broadcaster], 'L');
    while (!ss.empty())
    {
        auto [ni, in_idx, signal] = ss.front();
        ss.pop_front();
        auto& node = nodes[ni];
        switch (node.type)
        {
        case cNode::broadcaster:
            sendSignal(node, signal);
            break;
        case cNode::flip_flop:
/*
* If it was off, it turns on and sends a high pulse. If it was on, it turns off and sends a low pulse.
*/
            if (signal == 'L')
            {
                node.is_on = !node.is_on;
                sendSignal(node, node.is_on ? 'H' : 'L');
            }
            break;
        case cNode::conjunction:
//            Then, if it remembers high pulses for all inputs, it sends a low pulse; otherwise, it sends a high pulse.
            node.input_mem[in_idx] = signal;
            FOR(i, 4)
            {
                if (node.node_idx == checked_node_idx[i])
                {
                    if (find(ALL(node.input_mem), 'L') == node.input_mem.end())
                    {
                        P("%s all H at button #%lld (+%lld)", node.name.c_str(), button, button - last_all_h[i]);
                        last_all_h[i] = button;
//                        printConjuctionNode(button, node);
                    }
                }
            }
            sendSignal(node, find(ALL(node.input_mem), 'L') == node.input_mem.end() ? 'L' : 'H');
            break;
        }
    }
}

void solveFirst()
{
    P(">>> %lld <<<", lcm(vector<ll>{3797, 3733, 3907, 3823}));



    low_count = 0, high_count = 0;
    nodes.reserve(ls.size() + 1);
    names.clear();

    auto name_idx = [&](string name)
        {
            ll& i = names[name[0]=='&'||name[0]=='%'?name.substr(1):name];
            if (i == 0)
            {
                nodes.emplace_back();
                nodes.back().name = name[0] == '&' || name[0] == '%' ? name.substr(1) : name;
                i = (ll)nodes.size();
                nodes.back().node_idx = i - 1;
            }
            return i - 1;
        };

    ll res = 0;
    nodes.clear();

    for(auto& l: ls)
    {
        auto ni = name_idx(l.s[0]);
        for (auto& to : l.s | vs::drop(1))
        {
            auto target_idx = name_idx(to);
            cNode& node = nodes[ni];
            node.to.emplace_back(target_idx, 0);
        }
        cNode& node = nodes[ni];
        node.type = l.s[0][0] == '%' ? cNode::flip_flop : (l.s[0][0]=='&' ? cNode::conjunction : cNode::broadcaster);
    }
    broadcaster = names["broadcaster"] - 1;
    for (auto& node : nodes)
    {
        for (auto& [to, in_idx] : node.to)
        {
            auto& target_node = nodes[to];
            in_idx = (int)target_node.input_mem.size();
            target_node.input_mem.emplace_back('L'); // low
        }
    }

    checked_node_idx[0] = name_idx("ms");
    checked_node_idx[1] = name_idx("zt");
    checked_node_idx[2] = name_idx("xd");
    checked_node_idx[3] = name_idx("gt");

    FOR(step, 1'000'000)
    {
        simulate(step+1);
    }
    P("low count: %lld, high count: %lld, result: %lld\n", low_count, high_count, low_count* high_count);

    return;


    gotcha = false;

    nodes[name_idx("ms")].is_interesting = true;
    //nodes[name_idx("zt")].is_interesting = false;
    //nodes[name_idx("gt")].is_interesting = false;
    //nodes[name_idx("xd")].is_interesting = false;
  //  nodes[name_idx("ms")].is_interesting = false;
    //nodes[name_idx("zm")].is_interesting = false;
    //nodes[name_idx("mf")].is_interesting = false;
    //nodes[name_idx("qb")].is_interesting = false;
    //nodes[name_idx("fr")].is_interesting = false;
    //nodes[name_idx("fr")].is_interesting = false;
    //nodes[name_idx("nn")].is_interesting = false;
    //nodes[name_idx("jf")].is_interesting = false;
    //nodes[name_idx("fh")].is_interesting = false;
    //nodes[name_idx("br")].is_interesting = false;
    //nodes[name_idx("bv")].is_interesting = false;
    //nodes[name_idx("zq")].is_interesting = false;
    //nodes[name_idx("hg")].is_interesting = false;
    //nodes[name_idx("hr")].is_interesting = false;
    //nodes[name_idx("pd")].is_interesting = false;
    //nodes[name_idx("vr")].is_interesting = false;
    //nodes[name_idx("fx")].is_interesting = false;
    //nodes[name_idx("pm")].is_interesting = false;
    //nodes[name_idx("nz")].is_interesting = false;
    //nodes[name_idx("xh")].is_interesting = false;
    //nodes[name_idx("lt")].is_interesting = false;
    //nodes[name_idx("px")].is_interesting = false;
    //nodes[name_idx("vl")].is_interesting = false;
    //nodes[name_idx("xp")].is_interesting = false;
    //nodes[name_idx("mv")].is_interesting = false;
    //nodes[name_idx("mv")].is_interesting = false;
    //nodes[name_idx("ss")].is_interesting = false;
    //nodes[name_idx("hz")].is_interesting = false;
    //nodes[name_idx("nl")].is_interesting = false;
    //nodes[name_idx("vg")].is_interesting = false;
    //nodes[name_idx("tz")].is_interesting = false;
    //nodes[name_idx("qh")].is_interesting = false;
    //nodes[name_idx("bg")].is_interesting = false;
    //nodes[name_idx("bs")].is_interesting = false;
    //nodes[name_idx("gp")].is_interesting = false;
    //nodes[name_idx("kt")].is_interesting = false;
    //nodes[name_idx("jl")].is_interesting = false;
    //nodes[name_idx("sn")].is_interesting = false;
    //nodes[name_idx("xl")].is_interesting = false;
    //nodes[name_idx("xj")].is_interesting = false;
    //nodes[name_idx("bj")].is_interesting = false;
    //nodes[name_idx("cb")].is_interesting = false;
    //nodes[name_idx("kx")].is_interesting = false;
    //nodes[name_idx("hv")].is_interesting = false;

    cNode& checked_node = nodes[name_idx("xd")];

    for (auto& node : nodes)
    {
        if (node.type == cNode::conjunction)
        {
            P("node %s is receiving from: ", node.name.c_str());
            for (auto& source : nodes)
            {
                if (any_of(ALL(source.to), [&](auto& to) { return to.first == node.node_idx; }))
                {
                    PC("%s (%s), ", source.name.c_str(), source.type == cNode::conjunction ? "con" : "ff");
                }
            }
        }
    }

    vector<pair<bool, vector<ll>>> ffss;// state, last_switch
    ffss.resize(nodes.size()*10);
    for (auto ffs : ffss)
        ffs.second.emplace_back(0);
    //for (auto& node: nodes)
    //{
    // //   if (node.type == cNode::flip_flop)
    //    if(node.is_interesting)
    //    {
    //        ffss[node.node_idx].first = false;
    //        ffss[node.node_idx].second.emplace_back(0);
    //    }
    //}
    //name_idx("rx")

//    auto& checked_node

    FOR(i, 10'000'000)
    {


        simulate(i+1);
        for (auto& node : nodes)
        {
            //if (node.type == cNode::flip_flop)
            //{
            //    auto& [state, last_switch] = ffss[node.node_idx];
            //    if (last_switch.size() > 50)
            //        continue;
            //    if (state != node.is_on)
            //    {
            //        state = node.is_on;
            //        last_switch.emplace_back(i + 1);
            //    }
            //}
            if (node.is_interesting)
            {
                FOR(j, node.input_mem.size())
                {
                    auto& [state, last_switch] = ffss[node.node_idx * 10 + j];
                    if (last_switch.size() > 200)
                        continue;
                    bool current_state = node.input_mem[j] == 'H';
                    if (state != current_state)
                    {
                        state = current_state;
                        last_switch.emplace_back(i + 1);
                    }
                }
            }
        }



  //      P("--- after button #%lld", i + 1);
        //printState(i+1);
        printConjuctionNode(i + 1, checked_node);
//        printAllH(i + 1);

    }
    for (auto& node : nodes)
    {
        if (node.is_interesting)
        {
            P("NODE %s", node.name.c_str());
            FOR(i, node.input_mem.size())
            {
                P("---- bit %d ----", (int)i);
                auto& [state, last_switch] = ffss[node.node_idx * 10 + i];
                for (size_t i = 1; i < last_switch.size(); ++i)
                {
                    P("%s : %lld (+%lld)", i % 2 == 1 ? "ON " : "OFF", last_switch[i], last_switch[i] - last_switch[i - 1]);
                }
            }
            auto& [state, last_switch] = ffss[node.node_idx];
        }
        /*
        if (node.type == cNode::flip_flop)
        {
            auto& [state, last_switch] = ffss[node.node_idx];
            P("");
            P("NODE: %s", node.name.c_str());
            for (size_t i = 1; i < last_switch.size(); ++i)
            {
                P("%s : %lld (+%lld)", i % 2 == 1 ? "ON " : "OFF", last_switch[i], last_switch[i] - last_switch[i - 1]);
            }
        }
        */
    }


    P("low count: %lld, high count: %lld, result: %lld\n", low_count, high_count, low_count* high_count);
}

void solveSecond()
{
    ll res = 0;
    for (auto& l : ls)
    {
    }
    P("result: %lld", res);
}

 void solve(bool first) { first ? solveFirst() : solveSecond(); }
