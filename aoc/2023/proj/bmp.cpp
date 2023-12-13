#include "bmp.h"

void printImage(cImage<char>& img)
{
    cPosition pos;
    loop_row(pos, img)
    {
        loop_col(pos, img)
        {
            P("%c", img[pos]);
        }
        P("\n");
    }
    P("\n");
}