#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;


void move(cImage<char>& img, cPosition& robot_pos, cPosition direction)
{
    cPosition new_pos = robot_pos + direction;
    if (!img.isValidPos(new_pos) || img[new_pos] == '#')
    {
        return;
    }
    if (img[new_pos] == '.')
    {
        img[robot_pos] = '.';
        img[new_pos] = '@';
        robot_pos = new_pos;
        return;
    }
    cPosition hole = new_pos + direction;
    for (;;)
    {
        if (!img.isValidPos(hole) || img[hole] == '#')
        {
            return;
        }
        if (img[hole] == '.')
        {
            img[robot_pos] = '.';
            img[new_pos] = '@';
            img[hole] = 'O';
            robot_pos = new_pos;
            return;
        }
        hole += direction;
    }
}

void move2_h(cImage<char>& img, cPosition& robot_pos, cPosition direction)
{
    cPosition new_pos = robot_pos + direction;
    if (!img.isValidPos(new_pos) || img[new_pos] == '#')
    {
        return;
    }
    if (img[new_pos] == '.')
    {
        img[robot_pos] = '.';
        img[new_pos] = '@';
        robot_pos = new_pos;
        return;
    }
    cPosition hole = new_pos + direction;
    for (;;)
    {
        if (!img.isValidPos(hole) || img[hole] == '#')
        {
            return;
        }
        if (img[hole] == '.')
        {
            while (hole != robot_pos)
            {
                img[hole] = img[hole - direction];
                hole -= direction;
            }
            img[robot_pos] = '.';
            robot_pos = new_pos;
            return;
        }
        hole += direction;
    }
}



bool move2_v(cImage<char>& img, cPosition push_pos, cPosition direction)
{
    cPosition hole_pos = push_pos + direction;
    for (;; hole_pos += direction)
    {
        switch (img[hole_pos])
        {
        case '.':
            for (cPosition p = hole_pos; p != push_pos; p -= direction)
            {
                img[p] = img[p - direction];
            }
            img[push_pos] = '.';
            return true;
        case '#':
            return false;
        case '[':
            if (!move2_v(img, hole_pos + direction_E, direction))
            {
                return false;
            }
            break;
        case ']':
            if (!move2_v(img, hole_pos + direction_W, direction))
            {
                return false;
            }
            break;
        }
    }
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
            if (v == '#')
            {
                img2[p2] = '#';
                img2[p2 + cPosition(0, 1)] = '#';
            }
            else if (v == '@')
            {
                img2[p2] = '@';
                img2[p2 + cPosition(0, 1)] = '.';
            }
            else if (v == 'O')
            {
                img2[p2] = '[';
                img2[p2 + cPosition(0, 1)] = ']';
            }
        }
        img = img2;
        //if (is_example)
        //    printImage(img2);
        //return;
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
    P("robot_pos: {}, {}", robot_pos.row, robot_pos.col);

    for (auto&& [line_idx, l] : enumerate(blocks[1]))
    {
        auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;
        if (first)
        {
            for (auto c : l.txt)
            {
                switch (c)
                {
                case '^':
                    move(img, robot_pos, direction_N);
                    break;
                case 'v':
                    move(img, robot_pos, direction_S);
                    break;
                case '>':
                    move(img, robot_pos, direction_E);
                    break;
                case '<':
                    move(img, robot_pos, direction_W);
                    break;
                }
            }
        }
        else
        {
            for (auto c : l.txt)
            {
                switch (c)
                {
                case '^':
                {
                    cImage<char> undo(img);
                    if (!move2_v(img, robot_pos, direction_N))
                    {
                        img = undo;
                    }
                    else
                    {
                        robot_pos += direction_N;
                    }
                    break;
                }
                case 'v':
                {
                    cImage<char> undo(img);
                    if (!move2_v(img, robot_pos, direction_S))
                    {
                        img = undo;
                    }
                    else
                    {
                        robot_pos += direction_S;
                    }
                    break;
                }
                case '>':
                    move2_h(img, robot_pos, direction_E);
                    break;
                case '<':
                    move2_h(img, robot_pos, direction_W);
                    break;
                }
            }
        }
    }

    if (is_example)
        printImage(img);


    for (auto p : img.allPos())
    {
        if (img[p] == 'O' || img[p] == '[')
        {
            res += p.row * 100 + p.col;
        }
    }

    RESULT(res);
}