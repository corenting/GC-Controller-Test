#include "common_utils.h"
#include "version.h"
#include <stdio.h>

void DrawHeaderText(int activePad) {
  SetFgColor(2, 2);
  printf("GC Controller Test by corenting (version %d.%d) - ", V_MAJOR,
         V_MINOR);
#ifdef __WII__
  printf("Wii version\n\n");
#endif
#ifdef __GAMECUBE__
  printf("Gamecube version\n\n");
#endif
  SetFgColor(1, 2);
  printf("Current controller : %d (press a button on another controller to "
         "switch)\n\n\n",
         activePad + 1);
  SetFgColor(5, 2);
  printf("Special functions (hold the buttons) :\n\n");
  SetFgColor(7, 2);
  printf("    A + B : rumble test\n");
#ifdef __WII__
  printf("    L + R : return to the loader\n\n");
#endif
}

void SetFgColor(int color, int bold) {
  printf("\x1b[%u;%um", color + 30, bold);
  fflush(stdout);
}

void SetBgColor(int color, int bold) {
  printf("\x1b[%u;%um", color + 40, bold);
  fflush(stdout);
}

void SetPosition(int column, int row) { printf("\x1b[%d;%dH", row, column); }

char *GetPadDirection(short x, short y) {
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
