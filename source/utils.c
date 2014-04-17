/*
GC Controller Test
By corenting (http://www.corenting.fr)
*/

#include <stdio.h>
#include <gccore.h>
#include "utils.h"

void SetFgColor(u32 color, u8 bold)
{
    printf("\x1b[%u;%um", color + 30, bold);
    fflush(stdout);
}

void SetBgColor(u32 color, u8 bold)
{
    printf("\x1b[%u;%um", color + 40, bold);
    fflush(stdout);
}

void SetPosition(int column, int row)
{
    printf("\x1b[%d;%dH", row, column);
}

void LongWait(int waitTime)
{
    int i = 0;
    for (i; i < waitTime; i++)
    {
        VIDEO_WaitVSync();
    }
}