/*
GC Controller Test
By corenting (http://www.corenting.fr)
Version 1.0
*/


/*
TODO list :

	- Ajout sticks en tant que boutons (directions) :
		if(PAD_StickY(0) < -65)    {printf("You-Pressed Stick Analogic Down:\n");}
		else if(PAD_StickY(0) > 65)    {printf("You-Pressed Stick Analogic Up:\n");}
		if(PAD_StickX(0) < -65)    {printf("You-Pressed Stick Analogic Left:\n");}
		else if(PAD_StickX(0) > 65)    {printf("You-Pressed Stick Analogic Right:\n");}
	- Ajout dpad
	- Ajout support des 4 ports (variable pour le port, modifiable entre 1 et 4 avec une combinaison de touches)
	- Ajout ifndef (Gamecube/Wii) pour le retour vers le loader

*/
#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include "utils.h"

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
    console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

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
    printf("GC Controller Test by Corenting (version 1.0)\n\n");
    SetFgColor(1, 2);
    printf("You can only use the first Gamecube controller.\n\n\n");
    SetFgColor(5, 2);
    printf("Special functions :\n\n");
    SetFgColor(7, 2);
    printf("A + B : rumble test\n");
    printf("L + R : return to the loader\n\n");

    //Vars and consts
    int ABPressed[2] = {0, 0};
    int LRPressed[2] = {0, 0};
    int latestButtonsWait = 0;
    const int comboWait = 3;

    while (1)
    {

        // Call PAD_ScanPads each loop, this reads the latest controller states
        PAD_ScanPads();

        // WPAD_ButtonsDown tells us which buttons were pressed in this loop
        // this is a "one shot" state which will not fire again until the button has been released
        u32 GCHeld = PAD_ButtonsHeld(0);

        //Checking all the buttons
        SetPosition(0, 10);
        SetFgColor(5, 2);
        printf("Buttons :\n\n");
        SetFgColor(7, 2);
        printf("	A      : %s\n", (GCHeld & PAD_BUTTON_A) ? "Held    " : "        ");
        printf("	B      : %s\n", (GCHeld & PAD_BUTTON_B) ? "Held    " : "        ");
        printf("	X      : %s\n", (GCHeld & PAD_BUTTON_X) ? "Held    " : "        ");
        printf("	Y      : %s\n", (GCHeld & PAD_BUTTON_Y) ? "Held    " : "        ");
        printf("	Z      : %s\n", (GCHeld & PAD_TRIGGER_Z) ? "Held    " : "        ");
        printf("	L      : %s\n", (GCHeld & PAD_TRIGGER_L) ? "Held    " : "        ");
        printf("	R      : %s\n", (GCHeld & PAD_TRIGGER_R) ? "Held    " : "        ");
        printf("	START  : %s\n\n", (GCHeld & PAD_BUTTON_START) ? "Held    " : "        ");

        SetFgColor(5, 2);
        printf("Sticks and triggers (analog) :\n\n");
        SetFgColor(7, 2);
        printf("	L trigger           : %d\n", PAD_TriggerL(0));
        printf("	R trigger           : %d\n", PAD_TriggerR(0));
        printf("	Stick value (X,Y)   : %d,%d\n", PAD_StickX(0), PAD_StickY(0));
        printf("	C-stick value (X,Y) : %d,%d\n", PAD_SubStickX(0), PAD_SubStickY(0));

        //Add buttons to their list for special actions
        if (GCHeld & PAD_BUTTON_A)
        {
            ABPressed[0] = 1;
        }
        if (GCHeld & PAD_BUTTON_B)
        {
            ABPressed[1] = 1;
        }
        if (GCHeld & PAD_TRIGGER_L)
        {
            LRPressed[0] = 1;
        }
        if (GCHeld & PAD_TRIGGER_R)
        {
            LRPressed[1] = 1;
        }

        //Special actions
        if (ABPressed[0] == 1 && ABPressed[1] == 1)
        {
            PAD_ControlMotor(0, 1);
            latestButtonsWait = 0;
            ABPressed[0] = 0;
            ABPressed[1] = 0;
        }

        if (LRPressed[0] == 1 && LRPressed[1] == 1)
        {
            exit(0);
        }

        //Timer to clear the lists and stop the rumble
        if (latestButtonsWait < comboWait)
        {
            latestButtonsWait++;
        }
        else
        {
            latestButtonsWait = 0;
            int i;
            for (i = 0; i < 2; i++)
            {
                ABPressed[i] = 0;
                LRPressed[i] = 0;
            }
            PAD_ControlMotor(0, 0);
        }

        // Wait for the next frame
        LongWait(5);
    }

    return 0;
}
