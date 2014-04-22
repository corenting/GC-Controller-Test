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
    while (i < 5)
    {
        VIDEO_WaitVSync();
        i++;
    }
}

char *GetPadDirection(short x, short y)
{
    if (y < -65)
    {
        return "(down)";
    }
    else if (y > 65)
    {
        return "(up)";
    }
    else if (x < -65)
    {
        return "(left)";
    }
    else if (x > 65)
    {
        return "(right)";
    }
    else
    {
        return "";
    }
}