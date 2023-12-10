#include "aoc.h"
#include "bmp.h"

const char* main_delimeters = " ,";
bool main_allow_empty_fields = false;

void solve(bool first)
{
    auto img = loadImage(ls);
    cPosition startPos;
    [&]()
    {
        loop_row(startPos, img)
        {
            loop_col(startPos, img)
            {
                if (img[startPos] == 'S')
                    return;
            }
        }
    }();
    cPosition dir = direction_S;
    unordered_map<char, pair<cPosition, cPosition>> pipeMap;
    // | is a vertical pipe connecting north and south.
    pipeMap['|'] = make_pair(direction_N, direction_S);
    // - is a horizontal pipe connecting east and west.
    pipeMap['-'] = make_pair(direction_E, direction_W);
    // L is a 90 - degree bend connecting north and east.
    pipeMap['L'] = make_pair(direction_N, direction_E);
    // J is a 90 - degree bend connecting north and west.
    pipeMap['J'] = make_pair(direction_N, direction_W);
    // 7 is a 90 - degree bend connecting south and west.
    pipeMap['7'] = make_pair(direction_S, direction_W);
    // F is a 90 - degree bend connecting south and east.
    pipeMap['F'] = make_pair(direction_S, direction_E);

    auto changeDir = [&img, &pipeMap](cPosition pos, cPosition dir) -> cPosition
    {
        pair<cPosition, cPosition> pipe = pipeMap[img[pos]];
        if (-dir == pipe.first)
            return pipe.second;
        return pipe.first;
    };

    cPosition  pos = startPos;

//     img[startPos] = 'F'; // test
    img[startPos] = '7'; // real

    int steps = 0;
    cImage<char> isPipe;
    isPipe.cells.resize(img.cells.size() * 9, '.');
    isPipe.w = img.w * 3;
    isPipe.h = img.h * 3;
    do
    {
        cPosition p3c = pos * 3 + cPosition{ 1, 1 };

        isPipe.forOffsets(p3c, neighbour8Positions, [&](cPosition np) { isPipe[np] = ' '; });

        isPipe[p3c] = 'P';
        isPipe[p3c + pipeMap[img.at(pos)].first] = 'P';
        isPipe[p3c + pipeMap[img.at(pos)].second] = 'P';

        pos += dir;
        ++steps;
        dir = changeDir(pos, dir);
    }
    while (pos != startPos);

    vector<cPosition> f;
    f.emplace_back(0, 0);
    while (!f.empty())
    {
        cPosition p = f.back();
        f.pop_back();
        isPipe.forEach4Neighbour(p, [&](cPosition np)
            {
                char& c = isPipe[np];
                if (c == '.' || c==' ')
                {
                    f.emplace_back(np);
                    c = '#';
                }
            });
    }

//    isPipe.print();

    P("\n\nsteps: %d, half steps: %d\n", steps, steps / 2);
    P("inside: %lld\n", count(ALL(isPipe.cells), '.') / 9);
}


// void solveSecond()
// {
// }

// void solve(bool first) { first ? solveFirst() : solveSecond(); }
