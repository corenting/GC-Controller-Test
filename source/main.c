/*
GC Controller Test
By corenting (http://www.corenting.fr)
Version 1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include "utils.h"

#define consoleStartPos 20

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

int main(int argc, char **argv)
{
    // Initialise the video system
    VIDEO_Init();

    // This function initialises the attached controllers (GC and Wii)
    PAD_Init();

    // Obtain the preferred video mode from the system
    // This will correspond to the settings in the Wii menu
    rmode = VIDEO_GetPreferredMode(NULL);

    // Allocate memory for the display in the uncached region
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

    // Initialise the console, required for printf
    console_init(xfb, consoleStartPos, consoleStartPos, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

    // Set up the video registers with the chosen mode
    VIDEO_Configure(rmode);

    // Tell the video hardware where our display memory is
    VIDEO_SetNextFramebuffer(xfb);

    // Make the display visible
    VIDEO_SetBlack(FALSE);

    // Flush the video register changes to the hardware
    VIDEO_Flush();

    // Wait for Video setup to complete
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

    SetFgColor(2, 2);
    printf("GC Controller Test by Corenting (version 1.0) - ");
#ifdef WII
    printf("Wii version\n\n");
#endif
#ifdef GC
    printf("Gamecube version\n\n");
#endif
    SetFgColor(1, 2);
    printf("You can only use the first Gamecube controller.\n\n\n");
    SetFgColor(5, 2);
    printf("Special functions :\n\n");
    SetFgColor(7, 2);
    printf("A + B : rumble test\n");
#ifdef WII
    printf("L + R : return to the loader\n\n");
#endif

    //Vars and consts
    uint GCHeld = 0;
    uint GCHeldOld = 0;

    while (1)
    {

        //Stop rumble
        PAD_ControlMotor(0, 0);

        // Call PAD_ScanPads each loop, this reads the latest controller states
        PAD_ScanPads();

        // PAD_ButtonsDown tells us which buttons were pressed in this loop
        // this is a "one shot" state which will not fire again until the button has been released
        GCHeld = PAD_ButtonsHeld(0);

        //Go to the correct position
#ifdef WII
        SetPosition(0, 10);
#endif
#ifdef GC
        SetPosition(0, 9);
#endif

        //Checking all the buttons
        SetFgColor(5, 2);
        printf("Buttons :\n\n");
        SetFgColor(7, 2);
        printf("	A      : %s\n", (GCHeld & PAD_BUTTON_A) ? "held    " : "        ");
        printf("	B      : %s\n", (GCHeld & PAD_BUTTON_B) ? "held    " : "        ");
        printf("	X      : %s\n", (GCHeld & PAD_BUTTON_X) ? "held    " : "        ");
        printf("	Y      : %s\n", (GCHeld & PAD_BUTTON_Y) ? "held    " : "        ");
        printf("	Z      : %s\n", (GCHeld & PAD_TRIGGER_Z) ? "held    " : "        ");
        printf("	START  : %s\n", (GCHeld & PAD_BUTTON_START) ? "held    " : "        ");
        printf("	Pad    : ");
        if (GCHeld & PAD_BUTTON_LEFT)
        {
            printf("left ");
        }
        else if (GCHeld & PAD_BUTTON_RIGHT)
        {
            printf("right");
        }
        else if (GCHeld & PAD_BUTTON_UP)
        {
            printf("up   ");
        }
        else if (GCHeld & PAD_BUTTON_DOWN)
        {
            printf("down ");
        }
        else
        {
            printf("     ");
        }


        printf("\n\n");
        SetFgColor(5, 2);
        printf("Sticks and triggers (analog) :\n\n");
        SetFgColor(7, 2);
        printf("	L trigger           : %d  ", PAD_TriggerL(0));
        (GCHeld & PAD_TRIGGER_L) ? printf("(held)\n") : printf("        \n");
        printf("	R trigger           : %d  ", PAD_TriggerR(0));
        (GCHeld & PAD_TRIGGER_R) ? printf("(held)\n") : printf("        \n");
        printf("	Stick value (X,Y)   : %d,%d  %s        \n", PAD_StickX(0), PAD_StickY(0),GetPadDirection(PAD_StickX(0),PAD_StickY(0)));
        printf("	C-stick value (X,Y) : %d,%d  %s        \n", PAD_SubStickX(0), PAD_SubStickY(0),GetPadDirection(PAD_SubStickX(0),PAD_SubStickY(0)));


        //Special actions
        if (GCHeld & PAD_BUTTON_A && GCHeldOld & PAD_BUTTON_B)
        {
            PAD_ControlMotor(0, 1);
        }

#ifdef WII
        if (GCHeld & PAD_TRIGGER_L && GCHeldOld & PAD_TRIGGER_R)
        {
            exit(0);
        }
#endif


        //Store current state for the next iteration, for the special actions
        GCHeldOld = GCHeld;
        // Wait for 5 frames;
        LongWait(5);
    }
    exit(0);
}
