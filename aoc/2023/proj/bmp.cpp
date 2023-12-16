#include "bmp.h"

void printImage(cImage<char>& img)
{
    cPosition pos;
    pos.col = 0;
    loop_row(pos, img)
    {
        P("%.*s\n", img.w, &img[pos]);
//         loop_col(pos, img)
//         {
//             P("%c", img[pos]);
//         }
//         P("\n");
    }
    P("\n");
}