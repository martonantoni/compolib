#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

void move_shift(cImage<char>& img, cPosition& robot_pos, cDirection direction)
{
    for (cPosition hole = robot_pos + direction; img[hole] != '#'; hole += direction)
    {
        if (img[hole] == '.')
        {
            for (; hole != robot_pos; hole -= direction)
            {
                img[hole] = img[hole - direction];
            }
            img[robot_pos] = '.';
            robot_pos += direction;
            return;
        }
    }
}

bool move2_v(cImage<char>& img, cPosition push_pos,  cDirection direction)
{
    cPosition target_pos = push_pos + direction;
    switch (img[target_pos])
    {
    case '#':
        return false;
    case '[':
    case ']':
        if (!move2_v(img, target_pos + (img[target_pos] == '[' ?  cDirection::east() : cDirection::west()), direction))
            return false;
        if (!move2_v(img, target_pos, direction))
            return false;
    }
    img[target_pos] = img[push_pos];
    img[push_pos] = '.';
    return true;
}

void solve(bool first)
{
    ll res = 0;
    auto img = loadImage(blocks[0]);
    if (!first)
    {
        cImage<char> img2;
        img2.w = img.w * 2;
        img2.h = img.h;
        img2.cells.resize(img2.w * img2.h, '.');
        for (auto [p,v] : img.allPosV())
        {
            cPosition p2 = p;
            p2.col *= 2;
            switch (v)
            {
            case '#':
                img2[p2] = '#';
                img2[p2 + cPosition(0, 1)] = '#';
                break;
            case '@':
                img2[p2] = '@';
                break;
            case 'O':
                img2[p2] = '[';
                img2[p2 + cPosition(0, 1)] = ']';
                break;
            }
        }
        img = img2;
    }
    cPosition robot_pos;
    for(auto p: img.allPos())
    {
        if (img[p] == '@')
        {
            robot_pos = p;
            img[p] = '.';
            break;
        }
    }
    for (auto& l : blocks[1])
    {
        auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;
        if (first)
        {
            for (auto c : l.txt)
            {
                move_shift(img, robot_pos, cDirection::fromMark(c));
            }
        }
        else
        {
            for (auto c : l.txt)
            {
                switch (c)
                {
                case '^':
                case 'v':
                {
                    cImage<char> undo(img);
                    if (!move2_v(img, robot_pos, cDirection::fromMark(c)))
                    {
                        img = undo;
                    }
                    else
                    {
                        robot_pos += cDirection::fromMark(c);
                    }
                    break;
                }
                case '>':
                case '<':
                    move_shift(img, robot_pos, cDirection::fromMark(c));
                    break;
                }
            }
        }
    }
    for (auto p : img.allPos())
    {
        if (img[p] == 'O' || img[p] == '[')
        {
            res += p.row * 100 + p.col;
        }
    }
    RESULT(res);
}