#include "aoc.h"
#include "bmp.h"
#include "utils.h"

const char* outFileName = "aoc_out.txt";

const char* main_delimeters = " ,=pv";
bool main_allow_empty_fields = false;

void solve(bool first)
{
    ll res = 0;

    int w = is_example ? 11 : 101;
    int h = is_example ? 7 : 103;
    constexpr int steps = 100;

    vector<int> qc(4, 0);

    cImage<char> img;
    img.w = w;
    img.h = h;
    img.cells.resize(w * h, ' ');

    vector<cPosition> points;
    vector<cPosition> speeds;

    for (auto& l : ls)
    {
        auto& s = l.s; auto& is = l.i; auto& txt = l.txt; auto idx = l.idx;

        int x = l.i[0];
        int y = l.i[1];
        int vx = l.i[2];
        int vy = l.i[3];

        if (first)
        {
            int x2 = (l.i[0] + steps * vx + steps * w) % w;
            int y2 = (l.i[1] + steps * vy + steps * h) % h;

            if (x2 < w / 2 && y2 < h / 2)
            {
                qc[0]++;
            }
            else if (x2 > w / 2 && y2 < h / 2)
            {
                qc[1]++;
            }
            else if (x2 > w / 2 && y2 > h / 2)
            {
                qc[3]++;
            }
            else if (x2 != w / 2 && y2 != h / 2)
            {
                qc[2]++;
            }
        }
        else
        {

            points.push_back({ y, x });
            speeds.push_back({ vy + h, vx + w });
        }

    }

    if (first)
    {
        P("Quads: {}", qc);

        res = qc[0] * qc[1] * qc[2] * qc[3];

        RESULT(res);

    }
    else
    {
        if (is_example)
            return;
        for (int step = 0; step < w * h; ++step)
        {
            for (auto&& [p, v] : views::zip(points, speeds))
            {
                p += v;
                p.row %= h;
                p.col %= w;
                img[p] = '#';
            }
            if ((step - 803) % 101 == 0)
            {
                P("Step: {}", step + 1);
                printImage(img);
                P("");
                P("");
            }
            for (auto& p : points)
            {
                img[p] = ' ';
            }
        }
    }

}