/*
GC Controller Test
By corenting (http://www.corenting.fr)
*/

#include <stdio.h>
#include <gccore.h>
#include "utils.h"

void SetFgColor(uint color, ushort bold)
{
    printf("\x1b[%u;%um", color + 30, bold);
    fflush(stdout);
}

void SetBgColor(uint color, ushort bold)
{
    printf("\x1b[%u;%um", color + 40, bold);
    fflush(stdout);
}

void SetPosition(ushort column, ushort row)
{
    printf("\x1b[%d;%dH", row, column);
}

void LongWait(ushort waitTime)
{
    int i = 0;
    while (i < 5) {
        VIDEO_WaitVSync();
        i++;
    }
}

char *GetPadDirection(short x, short y)
{
    if (y < -50 && x < -50) {
        return "(down-left) ";
    }
    if (y < -50 && x > 50) {
        return "(down-right)";
    }
    if (y > 50 && x > 50) {
        return "(up-right)  ";
    }
    if (y > 50 && x < -50) {
        return "(up-left)   ";
    }
    if (y < -65) {
        return "(down)      ";
    }
    if (y > 65) {
        return "(up)        ";
    }
    if (x < -65) {
        return "(left)      ";
    }
    if (x > 65) {
        return "(right)     ";
    }
    return "            ";
}
