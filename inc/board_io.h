/***************************************************************************//**
 * @file
 * @brief Functions for using the on-board LEDs and buttons, if they exist
 ******************************************************************************/


#pragma once
#include <stdbool.h>

void board_io_init();
bool board_io_get_button0();
bool board_io_get_button1();
void board_io_set_led0(bool state);
void board_io_set_led1(bool state);
