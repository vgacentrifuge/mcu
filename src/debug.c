#include "debug.h"

#include "sl_debug_swo.h"
#include "efm32gg990f1024.h"
#include <stdio.h>

// We use the SWO Debug package to send data over the SWO line
// which is PF7 on both the dev board and our PCB

void debug_print(char* str) {
  for(; *str != '\0'; str++)
    sl_debug_swo_write_u8(0, *str);
}

void debug_println(char* str) {
  debug_print(str);
  sl_debug_swo_write_u8(0, '\n');
}

void debug_printintln(int i) {
  char out[20];
  snprintf(out, sizeof(out), "%d", i);
  debug_println(out);
}
