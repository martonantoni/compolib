#include "aoc.h"
#include "bmp.h"
#include "utils.h"

#include <random>

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,:->";
bool main_allow_empty_fields = false;


void solve(bool first)
{
    ll res = 0;

    vector<array<int, 5>> locks;
    vector<array<int, 5>> keys;

    for (auto& b : blocks)
    {
        auto img = loadImage(b);
        if (img[cPosition(0, 0)] == '#')
        {
            // lock
            auto& lock = locks.emplace_back();
            for (int i = 0; i < 5; ++i)
            {
                int h = 0;
                while (img[cPosition(h, i)] == '#')
                    ++h;
                lock[i] = h - 1;
            }
        }
        else
        {
            // key
            auto& key = keys.emplace_back();
            for (int i = 0; i < 5; ++i)
            {
                int h = 0;
                while (img[cPosition(6 - h, i)] == '#')
                    ++h;
                key[i] = h - 1;
            }
        }
    }

    for (auto& key : keys)
    {
        for (auto& lock : locks)
        {
            bool match = true;
            for (int i = 0; i < 5; ++i)
            {
                if (key[i] + lock[i] > 5)
                {
                    match = false;
                    break;
                }
            }
            if (match)
            {
                ++res;
            }
        }
    }



    RESULT(res);

}
