/*
GC Controller Test
By corenting
*/

#include <string.h>
#include <stdio.h>

#include <switch.h>

#include "../common/common_utils.h"


int main(int argc, char **argv) {
    // Init
    consoleInit(NULL);
    u64 keysHeld = 0;

    // Header text
    DrawHeaderText(0);

    // Main loop
    while (appletMainLoop()) {
        // Get inputs
        hidScanInput();
        keysHeld = hidKeysHeld(CONTROLLER_P1_AUTO);
        JoystickPosition pos_left, pos_right;
        hidJoystickRead(&pos_left, CONTROLLER_P1_AUTO, JOYSTICK_LEFT);
        hidJoystickRead(&pos_right, CONTROLLER_P1_AUTO, JOYSTICK_RIGHT);

        SetPosition(0, 10);

        // Buttons
        SetFgColor(5, 2);
        printf("Buttons:\n\n");
        SetFgColor(7, 2);
        printf((keysHeld & KEY_A) ? "    A " : "      ");
        printf((keysHeld & KEY_B) ? "B " : "  ");
        printf((keysHeld & KEY_X) ? "X " : "  ");
        printf((keysHeld & KEY_Y) ? "Y " : "  ");
        printf((keysHeld & KEY_ZR) ? "Z " : "  ");
        printf((keysHeld & KEY_L) ? "L " : "  ");
        printf((keysHeld & KEY_R) ? "R " : "  ");
        printf((keysHeld & KEY_PLUS) ? "START " : "      ");
        printf((keysHeld & KEY_DLEFT) ? "LEFT " : "     ");
        printf((keysHeld & KEY_DRIGHT) ? "RIGHT " : "      ");
        printf((keysHeld & KEY_DUP) ? "UP " : "   ");
        printf((keysHeld & KEY_DDOWN) ? "DOWN " : "     ");

        if (keysHeld & KEY_L & KEY_R) {
            break;
        }

        //Print the joysticks' position
        //printf("\x1b[3;1H%04d; %04d", pos_left.dx, pos_left.dy);
        //printf("\x1b[5;1H%04d; %04d", pos_right.dx, pos_right.dy);

        // Prepare for next frame
        consoleUpdate(NULL);
    }

    consoleExit(NULL);
    return 0;
}
