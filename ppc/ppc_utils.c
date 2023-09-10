#include <stdio.h>

#include <gccore.h>

#include "ppc_utils.h"

void LongWait(int waitTime) {
  int i = 0;
  while (i < waitTime) {
    VIDEO_WaitVSync();
    i++;
  }
}
