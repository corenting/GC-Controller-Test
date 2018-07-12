/*
GC Controller Test
By corenting (http://www.corenting.fr)
Version 1.2
*/

#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>

#include "utils.h"

#define CONSOLE_START_POS 20
#define V_MAJOR 1
#define V_MINOR 2

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

int main(int argc, char **argv)
{
    // Init
    VIDEO_Init();
    PAD_Init();
    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    console_init(xfb, CONSOLE_START_POS, CONSOLE_START_POS, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);
    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();

    // Wait for Video setup to complete
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

    // Vars
    short activePad = 0;
    uint GCHeld[4] = {0, 0, 0, 0};
    uint GCHeldOld[4] = {0, 0, 0, 0};

    // Header text
    SetFgColor(2, 2);
    printf("GC Controller Test by Corenting (version %d.%d) - ",V_MAJOR,V_MINOR);
#ifdef WII
    printf("Wii version\n\n");
#endif
#ifdef GC
    printf("Gamecube version\n\n");
#endif
    SetFgColor(1, 2);
    printf("Current controller : %d (press a button on another controller to switch)\n\n\n",activePad + 1);
    SetFgColor(5, 2);
    printf("Special functions (hold the buttons) :\n\n");
    SetFgColor(7, 2);
    printf("    A + B : rumble test\n");
#ifdef WII
    printf("    L + R : return to the loader\n\n");
#endif

    while (1) {
        PAD_ScanPads();
        for(int i = 0; i < 4; i++) {
            PAD_ControlMotor(i, 0); //Stop rumble
            GCHeld[i] = PAD_ButtonsHeld(i);
        }

        //Go to the correct position
#ifdef WII
        SetPosition(0, 10);
#endif
#ifdef GC
        SetPosition(0, 9);
#endif

        // Buttons
        SetFgColor(5, 2);
        printf("Buttons:\n\n");
        SetFgColor(7, 2);
        printf((GCHeld[activePad] & PAD_BUTTON_A) ? "    A " : "      ");
        printf((GCHeld[activePad] & PAD_BUTTON_B) ? "B " : "  ");
        printf((GCHeld[activePad] & PAD_BUTTON_X) ? "X " : "  ");
        printf((GCHeld[activePad] & PAD_BUTTON_Y) ? "Y " : "  ");
        printf((GCHeld[activePad] & PAD_TRIGGER_Z) ? "Z " : "  ");
        printf((GCHeld[activePad] & PAD_TRIGGER_L) ? "L " : "  ");
        printf((GCHeld[activePad] & PAD_TRIGGER_R) ? "R " : "  ");
        printf((GCHeld[activePad] & PAD_BUTTON_START) ? "START " : "      ");
        printf((GCHeld[activePad] & PAD_BUTTON_LEFT) ? "LEFT " : "     ");
        printf((GCHeld[activePad] & PAD_BUTTON_RIGHT) ? "RIGHT " : "      ");
        printf((GCHeld[activePad] & PAD_BUTTON_UP) ? "UP " : "   ");
        printf((GCHeld[activePad] & PAD_BUTTON_DOWN) ? "DOWN " : "     ");

        // Sticks and triggers
        printf("\n\n");
        SetFgColor(5, 2);
        printf("Sticks and triggers (analog) :\n\n");
        SetFgColor(7, 2);
        printf("	L trigger           : %03d\n", PAD_TriggerL(activePad));
        printf("	R trigger           : %03d\n", PAD_TriggerR(activePad));
        printf("	Stick value (X,Y)   : %03d,%03d %s\n", PAD_StickX(activePad), PAD_StickY(activePad), GetPadDirection(PAD_StickX(activePad), PAD_StickY(activePad)));
        printf("	C-stick value (X,Y) : %03d,%03d %s\n", PAD_SubStickX(activePad), PAD_SubStickY(activePad), GetPadDirection(PAD_SubStickX(activePad), PAD_SubStickY(activePad)));


        //Special actions
        if (GCHeld[activePad] & PAD_BUTTON_A && GCHeldOld[activePad] & PAD_BUTTON_B) {
            PAD_ControlMotor(activePad, 1);
        }
#ifdef WII
        if (GCHeld[activePad] & PAD_TRIGGER_L && GCHeldOld[activePad] & PAD_TRIGGER_R) {
            exit(0);
        }
#endif

        //Active controller modifier
        for(int i = 0; i < 4; i++) {
            if (GCHeld[i] != 0) {
                activePad = i;
                SetPosition(21, 2);
                SetFgColor(1, 2);
                printf("%d", activePad + 1);
            }
        }

        //Store current state for the next iteration, for the special actions
        for(int i = 0; i < 4; i++) {
            GCHeldOld[i] = GCHeld[i];
        }
        // Wait for 5 frames;
        LongWait(5);
    }
    exit(0);
}
