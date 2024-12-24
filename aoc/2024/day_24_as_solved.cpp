#include "aoc.h"
#include "bmp.h"
#include "utils.h"

#include <random>

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,:->";
bool main_allow_empty_fields = false;

struct cWire
{
    ll v = 0;
    vector<int> used_by; // nodes
};

struct cNode
{
    ll v = 0;
    int c = 0;
    int target_wire;
    string target_wire_name;
    char op;
};

vector<cWire> wires;
vector<cNode> nodes;

void solve(bool first)
{
    if (!first)
        return;
    if (is_example)
        return;

    wires.clear();
    nodes.clear();
    wires.resize(ls.size());


//    names.resize(ls.size());
    //unordered_map<string, int> nodeIDs;
    //auto node_id2idx = [&](const string& id) -> int
    //    {
    //        auto it = nodeIDs.find(id);
    //        if (it == nodeIDs.end())
    //        {
    //            int idx = (int)nodeIDs.size();
    //            nodeIDs[id] = idx;
    //            return idx;
    //        }
    //        return it->second;
    //    };
    unordered_map<string, int> wireIDs;
    auto wire_id2idx = [&](const string& id) -> int
        {
            auto it = wireIDs.find(id);
            if (it == wireIDs.end())
            {
                int idx = (int)wireIDs.size();
                wires.emplace_back();
                wireIDs[id] = idx;
                return idx;
            }
            return it->second;
        };

    ll res = 0;

    vector<int> x_wires, y_wires, z_wires;

    x_wires.resize(64);
    y_wires.resize(64);
    z_wires.resize(64);
    auto extractNum = [](const string & s) -> int
    {
        return (s[1] - '0') * 10 + s[2] - '0';
    };
    vector<int> start_f;

    int num_bits = 0;
    for (auto& l : blocks[0])
    {
        int wireId = wire_id2idx(l.s[0]);
        wires[wireId].v = l.i[1];
        int i = extractNum(l.s[0]);
        num_bits = max(num_bits, i + 1);
        if (l.txt[0] == 'x')
        {
            x_wires[i] = wireId;
        }
        else if (l.txt[0] == 'y')
        {
            y_wires[i] = wireId;
        }
        start_f.emplace_back(wireId);
    }
    int start_wire_count = num_bits * 2;
//    int num_bits = x_wires.size();
    x_wires.resize(num_bits);
    y_wires.resize(num_bits);
    z_wires.resize(num_bits + 1);
    int start_wires = (int)wireIDs.size();
    P("start wires: {}", start_wires);

    // ntg XOR fgs -> mjb
    //  0   1   2      3

    for (auto& l : blocks[1])
    {
        int wireId = wire_id2idx(l.s[3]);
        if (l.s[3][0] == 'z')
        {
       //     P("z wire: {}", l.s[3]);
            z_wires[extractNum(l.s[3])] = wireId;
          //  PC("  extracted: {}, wireId: {}", extractNum(l.s[3]), wireId);
        }
        int nodeId = (int)nodes.size();
        auto& node = nodes.emplace_back();
        node.op = l.s[1][0];
        node.target_wire = wireId;
        node.target_wire_name = l.s[3];

        int sourceWireId1 = wire_id2idx(l.s[0]);
        int sourceWireId2 = wire_id2idx(l.s[2]);
        wires[sourceWireId1].used_by.emplace_back(nodeId);
        wires[sourceWireId2].used_by.emplace_back(nodeId);
    }
    int wire_count = (int)wireIDs.size();
    wires.resize(wire_count);
    int node_count = (int)nodes.size();
    P("wire count: {}", wire_count);
    P("node count: {}", node_count);

    int max_bit_to_check = 3;

    auto run = [&](ll x, ll y) -> bool
        {
            for (auto zw : z_wires)
            {
                wires[zw].v = -1;
            }
            for(auto& node: nodes)
            {
                node.v = 0;
                node.c = 0;
            }
            for (int i = 0; i < num_bits; ++i)
            {
                wires[x_wires[i]].v = (x >> i) & 1ll;
                wires[y_wires[i]].v = (y >> i) & 1ll;
            }
            vector<int> f = start_f;


            while (!f.empty())
            {
                auto w_idx = f.back();
                f.pop_back();
                auto& wire = wires[w_idx];
              //  P("wire[{}].v = {}", w_idx, wire.v);
                for (auto used_by : wire.used_by)
                {
                    auto& node = nodes[used_by];
                    node.c++;
                    if (node.c == 2)
                    {
                        switch (node.op)
                        {
                        case 'X':
                            node.v ^= wire.v;
                            break;
                        case 'O':
                            node.v |= wire.v;
                            break;
                        case 'A':
                            node.v &= wire.v;
                            break;
                        }
                        auto& target_wire = wires[node.target_wire];
                    //    P("node[{}].v = {}", used_by, node.v);  
                        target_wire.v = node.v;
                        f.emplace_back(node.target_wire);
                    }
                    else
                    {
                        node.v = wire.v;
                    }
                }
            }
            ll res = 0;
            for (int i = 0; i < num_bits + 1; ++i)
            {
                auto& wire = wires[z_wires[i]];
                if (wire.v == -1)
                {
                    return false;
                }
         //       P("wire[{}].v = {}", i, wire.v);
                res |= (wires[z_wires[i]].v << i);
            }
            //P("x = {}, y = {}, expected: {}, res = {}", x, y, x + y, res);
            return res == x + y;
        };


    bool debugPrint = false;

    auto runNbits = [&](int n) -> bool
        {
            for (int i = 0; i < n; ++i)
            {
                //if(debugPrint)
                //    P("i = {}", i);
                if (!run(1ll << i, 1ll << i) ||
                    !run(1ll << i, 0) ||
                    !run(0, 1ll << i))
                {
                    return false;
                }
            }
            return true;
        };

    debugPrint = true;

    //P("NO SWAP RUNS");

    //for (int bit = 0; bit < num_bits; ++bit)
    //{
    //    max_bit_to_check = bit + 1;
    //    if (!runNbits(bit))
    //    {
    //        P("failed on bit = {}", bit);
    //        break;
    //    }
    //}

    auto swapWires = [&](int a, int b)
        {
            auto& na = nodes[a];
            auto& nb = nodes[b];
            swap(na.target_wire, nb.target_wire);
        };

    //P("1 SWAP RUNS");
    //for (int i0 = 0; i0 < node_count; ++i0)
    //{
    //    for (int i1 = i0 + 1; i1 < node_count; ++i1)
    //    {
    //        swapWires(i0, i1);
    //        if (runNbits(max_bit_to_check))
    //        {
    //            P("i0 = {}, i1 = {}, fixed", i0, i1);                
    //        }
    //        swapWires(i0, i1);
    //    }
    //}

//    swapWires(71, 168);
//    for(int bits_good_until = 

    //auto tryFix = [&]() -> pair<int,int>
    //    {
    //        pair<int, int> result = { -1, -1 };
    //        for (;;)
    //        {
    //            int fix_count = 0;
    //            for (int i0 = 0; i0 < node_count; ++i0)
    //            {
    //                for (int i1 = i0 + 1; i1 < node_count; ++i1)
    //                {
    //                    swapWires(i0, i1);
    //                    if (runNbits(max_bit_to_check))
    //                    {
    //                        P("i0 = {}, i1 = {}, fixed", i0, i1);
    //                        ++fix_count;
    //                        result = { i0, i1 };
    //                    }
    //                    swapWires(i0, i1);
    //                }
    //            }
    //            if (fix_count == 1)
    //                break;
    //            if (fix_count == 0)
    //            {
    //                P("PANIC: no fix found");
    //                exit(0);
    //            }
    //            P("too many fixes, trying higher bits");
    //            ++max_bit_to_check;
    //        }
    //        return result;
    //    };


    //for (int s_count = 0; s_count < 2; ++s_count)
    //{
    //    P("swap #{}", s_count);
    //    // find first bit where it breaks:
    //    for (int bit = 0; bit < num_bits; ++bit)
    //    {
    //        max_bit_to_check = bit + 1;
    //        if (!runNbits(bit))
    //        {
    //            P("failed on bit = {}", bit);
    //            break;
    //        }
    //    }
    //    // swap wires until fixed:
    //    auto [first, second] = tryFix();
    //    swapWires(first, second);
    //}
    //max_bit_to_check = num_bits;
    //for (int i0a = 0; i0a < node_count; ++i0a)
    //{
    //    printf("."); fflush(stdout);
    //    for (int i0b = i0a + 1; i0b < node_count; ++i0b)
    //    {
    //        swapWires(i0a, i0b);
    //        for (int i1a = i0a + 1; i1a < node_count; ++i1a)
    //        {
    //            for (int i1b = i1a + 1; i1b < node_count; ++i1b)
    //            {
    //                swapWires(i1a, i1b);
    //                if (runNbits(max_bit_to_check))
    //                {
    //                    P("i0a = {}, i0b = {}, i1a = {}, i1b = {}, fixed", i0a, i0b, i1a, i1b);
    //                    printf("i0a = %d, i0b = %d, i1a = %d, i1b = %d, fixed\n", i0a, i0b, i1a, i1b);
    //                }
    //                swapWires(i1a, i1b);
    //            }
    //        }
    //        swapWires(i0a, i0b);
    //    }
    //}

    vector<array<pair<int, int>, 4>> swaps;
    swaps.resize(3);
    for (auto& s : swaps)
    {
        s[0] = { 71, 168 };
        s[1] = { 65, 199 };
    }
    swaps[0][2] = { 29, 206 };
    swaps[0][3] = { 103, 196 };
    swaps[1][2] = { 29, 206 };
    swaps[1][3] = { 165, 196 };
    swaps[2][2] = { 103, 196 };
    swaps[2][3] = { 124, 206 };
    //swaps[3][2] = { 123, 206 };
    //swaps[3][3] = { 165, 196 };


        auto rd = std::random_device();
        auto gen = std::mt19937(rd());
        auto dis = std::uniform_int_distribution<ll>(0, 1ll << num_bits);
    
        max_bit_to_check = num_bits;
 
    for (auto& s : swaps)
    {
        for (auto& p : s)
        {
            swapWires(p.first, p.second);
        }
        bool all_good = true;
        ll max = 1ll << (max_bit_to_check + 1);
        for (int i = 0; i < 1'000'000; ++i)
        {
            ll a = dis(gen);
            ll b = dis(gen);
            if (!run(a, b))
            {
                all_good = false;
                P("failed on test #{}: a = {}, b = {}", i, a, b);
                break;
            }
        }
        if (all_good)
        {
            vector<string> swapped_wires;
            for (auto& p : s)
            {
                swapped_wires.emplace_back(nodes[p.first].target_wire_name);
                swapped_wires.emplace_back(nodes[p.second].target_wire_name);
            }
            sort(ALL(swapped_wires));
            P("GOOD swapped wires: ");
            for (auto& s : swapped_wires)
            {
                PC("{},", s);
            }
        }
        for (auto& p : s)
        {
            swapWires(p.first, p.second);
        }
    }


}
