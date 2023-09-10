#include <stdio.h>
#include <stdlib.h>

#include <gccore.h>

#include "../common/common_utils.h"
#include "ppc_utils.h"

#define CONSOLE_START_POS 20

static void *xfb = NULL;
static GXRModeObj *rMode = NULL;

int main(int argc, char **argv) {
  // Init
  VIDEO_Init();
  PAD_Init();
  rMode = VIDEO_GetPreferredMode(NULL);
  xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rMode));
  console_init(xfb, CONSOLE_START_POS, CONSOLE_START_POS, rMode->fbWidth,
               rMode->xfbHeight, rMode->fbWidth * VI_DISPLAY_PIX_SZ);
  VIDEO_Configure(rMode);
  VIDEO_SetNextFramebuffer(xfb);
  VIDEO_SetBlack(FALSE);
  VIDEO_Flush();

  // Wait for video setup to complete
  VIDEO_WaitVSync();
  if (rMode->viTVMode & VI_NON_INTERLACE)
    VIDEO_WaitVSync();

  // Vars
  int activePad = 0;
  u16 keysHeld[4] = {0, 0, 0, 0};
  u16 keysHeldOld[4] = {0, 0, 0, 0};

  DrawHeaderText(activePad);
  while (1) {
    PAD_ScanPads();
    for (int i = 0; i < 4; i++) {
      PAD_ControlMotor(i, 0); // stop rumble
      keysHeld[i] = PAD_ButtonsHeld(i);
    }

    // Go to the correct position to draw
#ifdef __WII__
    SetPosition(0, 10);
#endif
#ifdef __GAMECUBE__
    SetPosition(0, 9);
#endif
    // Buttons
    SetFgColor(5, 2);
    printf("Buttons:\n\n");
    SetFgColor(7, 2);
    printf((keysHeld[activePad] & PAD_BUTTON_A) ? "    A " : "      ");
    printf((keysHeld[activePad] & PAD_BUTTON_B) ? "B " : "  ");
    printf((keysHeld[activePad] & PAD_BUTTON_X) ? "X " : "  ");
    printf((keysHeld[activePad] & PAD_BUTTON_Y) ? "Y " : "  ");
    printf((keysHeld[activePad] & PAD_TRIGGER_Z) ? "Z " : "  ");
    printf((keysHeld[activePad] & PAD_TRIGGER_L) ? "L " : "  ");
    printf((keysHeld[activePad] & PAD_TRIGGER_R) ? "R " : "  ");
    printf((keysHeld[activePad] & PAD_BUTTON_START) ? "START " : "      ");
    printf((keysHeld[activePad] & PAD_BUTTON_LEFT) ? "LEFT " : "     ");
    printf((keysHeld[activePad] & PAD_BUTTON_RIGHT) ? "RIGHT " : "      ");
    printf((keysHeld[activePad] & PAD_BUTTON_UP) ? "UP " : "   ");
    printf((keysHeld[activePad] & PAD_BUTTON_DOWN) ? "DOWN " : "     ");

    // Sticks and triggers
    printf("\n\n");
    SetFgColor(5, 2);
    printf("Sticks and triggers (analog) :\n\n");
    SetFgColor(7, 2);
    printf("	L trigger           : %03d\n", PAD_TriggerL(activePad));
    printf("	R trigger           : %03d\n", PAD_TriggerR(activePad));
    printf("	Stick value (X,Y)   : %03d,%03d %s\n", PAD_StickX(activePad),
           PAD_StickY(activePad),
           GetPadDirection(PAD_StickX(activePad), PAD_StickY(activePad)));
    printf("	C-stick value (X,Y) : %03d,%03d %s\n", PAD_SubStickX(activePad),
           PAD_SubStickY(activePad),
           GetPadDirection(PAD_SubStickX(activePad), PAD_SubStickY(activePad)));

    // Special actions
    if (keysHeld[activePad] & PAD_BUTTON_A &&
        keysHeldOld[activePad] & PAD_BUTTON_B) {
      PAD_ControlMotor(activePad, 1);
    }
#ifdef __WII__
    if (keysHeld[activePad] & PAD_TRIGGER_L &&
        keysHeldOld[activePad] & PAD_TRIGGER_R) {
      exit(0);
    }
#endif

    // Active controller modifier
    for (int i = 0; i < 4; i++) {
      if (keysHeld[i] != 0) {
        activePad = i;
        SetPosition(21, 2);
        SetFgColor(1, 2);
        printf("%d", activePad + 1);
      }
    }

    // Store current state for the next iteration, for the special actions
    for (int i = 0; i < 4; i++) {
      keysHeldOld[i] = keysHeld[i];
    }
    LongWait(5); // wait 5 frames for the next draw
  }
  exit(0);
}
