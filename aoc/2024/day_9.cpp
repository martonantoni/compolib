#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,:";
bool main_allow_empty_fields = false;

void solve(bool first)
{
    if (first)
        solveFirst();
    else
        solveSecond();
    //    auto img = loadImage(ls);
}

void solveFirst()
{
    ll res = 0;

    //for (auto& l : ls)
    //{
    //    auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;
    //}

    auto& l = ls[0].txt;
    vector<int> blocks;
    int idx = 0;
    vector<int> empty_places;
    for (auto&& [step, c] : views::enumerate(l))
    {
        if (step % 2 == 0)
        {
            for (int i = 0; i < c - '0'; ++i)
                blocks.emplace_back(idx);
            ++idx;
        }
        else
        {
            for (int i = 0; i < c - '0'; ++i)
            {
                empty_places.emplace_back((int)blocks.size());
                blocks.emplace_back(-1);
            }
        }
    }
    reverse(ALL(empty_places));
    for (int j = blocks.size() - 1; j >= 0; --j)
    {
        if (blocks[j] != -1)
        {
            if (!empty_places.empty())
            {
                if (empty_places.back() > j)
                    break;
                blocks[empty_places.back()] = blocks[j];
                empty_places.pop_back();
                blocks[j] = -1;
            }
            else
                break;
        }
    }


    for (int i = 0; i < blocks.size(); ++i)
    {
        if (is_example)
        {
            if (blocks[i] == -1)
                PC(".");
            else
                PC("{}", blocks[i]);
        }
        if (blocks[i] != -1)
            res += i * blocks[i];
    }

    P("Result: {}\n", res);
}

void solveSecond()
{

    ll res = 0;

    //for (auto& l : ls)
    //{
    //    auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;
    //}

    auto& l = ls[0].txt;
    vector<int> blocks;
    int idx = 0;
    vector<pair<int, int>> empty_places; // place, length
    vector<pair<int, int>> files; // place, length
    for (auto&& [step, c] : views::enumerate(l))
    {
        if (step % 2 == 0)
        {
            files.emplace_back((int)blocks.size(), c - '0');
            for (int i = 0; i < c - '0'; ++i)
                blocks.emplace_back(idx);
            ++idx;
        }
        else
        {
            empty_places.emplace_back((int)blocks.size(), c - '0');
            for (int i = 0; i < c - '0'; ++i)
                blocks.emplace_back(-1);
        }
    }
    reverse(ALL(files));

    for (auto&& [file_pos, file_length] : files)
    {
        for (auto&& [empty_pos, empty_length] : empty_places)
        {
            if (empty_length >= file_length)
            {
                if (empty_pos >= file_pos)
                    break;
                int file_idx = blocks[file_pos];
                for (int i = 0; i < file_length; ++i)
                {
                    blocks[i + empty_pos] = file_idx;
                    blocks[i + file_pos] = -1;
                }
                empty_pos += file_length;
                empty_length -= file_length;
                break;
            }
        }
    }

    for (int i = 0; i < blocks.size(); ++i)
    {
        if (is_example)
        {
            if (blocks[i] == -1)
                PC(".");
            else
                PC("{}", blocks[i]);
        }
        if (blocks[i] != -1)
            res += i * blocks[i];
    }

    P("Result: {}\n", res);
}

//for (auto& l : ls)
//{
//    auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;
//}