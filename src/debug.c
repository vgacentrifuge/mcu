#include "debug.h"

#include "sl_debug_swo.h"
#include "efm32gg990f1024.h"
#include <stdio.h>

// We use the SWO Debug package to send data over the SWO line
// which is PF7 on both the dev board and our PCB

char print_buffer[4096];
int buffer_used;

void debug_flush() {
  for(int i = 0; i < buffer_used; i++)
    sl_debug_swo_write_u8(0, print_buffer[i]);
  buffer_used = 0;
}

void debug_printchar(char c) {
  if(buffer_used == sizeof(print_buffer)) {
    debug_flush(); debug_print("<force flush>");
  }
  print_buffer[buffer_used++] = c;
}

void debug_print(char* str) {
  for(; *str != '\0'; str++)
    debug_printchar(*str);
}

void debug_println(char* str) {
  debug_print(str);
  debug_printchar('\n');
}

void debug_printintln(int i) {
  char out[20];
  snprintf(out, sizeof(out), "%d", i);
  debug_println(out);
}

void debug_printhexbyte(uint8_t byte) {
  char out[20];
  snprintf(out, sizeof(out), "%x ", byte);
  debug_print(out);
}

void debug_printhexbytesln(uint8_t* bytes, int len) {
  for(int i = 0; i < len; i++) {
      debug_printhexbyte(bytes[i]);
  }
  debug_printchar('\n');
}
