/***************************************************************************//**
 * @file
 * @brief Debugging helpers
 ******************************************************************************/

#pragma once

#include <stdint.h>

void debug_flush();
void debug_printchar(char c);
void debug_print(char* str);
void debug_println(char* str);
void debug_printintln(int i);
void debug_printhexbyte(uint8_t byte);
void debug_printhexbytesln(uint8_t* bytes, int len);

#define DEBUG_HERE() do { \
  debug_print(__FILE__); \
  debug_printchar(':');  \
  debug_printintln(__LINE__); \
  } while(0)
