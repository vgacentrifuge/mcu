/***************************************************************************//**
 * @file
 * @brief Functions for taking input from the 4x4 keypad
 *
 * Use the KEY_ macros to get key indexes for the different keys.
 * Keys are 0-indexed and row major.
 ******************************************************************************/

#pragma once

#include <stdbool.h>

#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4
#define KEY_INDEX(x, y) ((x)+(y)*KEYPAD_COLS)
#define KEY_UP KEY_INDEX(2, 2)
#define KEY_LEFT KEY_INDEX(1, 3)
#define KEY_DOWN KEY_INDEX(2, 3)
#define KEY_RIGHT KEY_INDEX(3, 3)

void keypad_init();

void keypad_sample();

bool keypad_keydown(int key);

bool keypad_keypressed(int key);

bool keypad_keyreleased(int key);
