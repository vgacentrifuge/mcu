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

/**
 * Samples the keypad, run once at the start of each frame, before using the other keypad functions.
 * Should be run at a mostly constant frequency, to make "frames held" a usable measure.
 */
void keypad_sample();

/**
 * Returns true if the button is considered down.
 * The button is down the same frame as keypad_keypressed() is true,
 * and is no longer down when keypad_keyreleased() returns true.
 * @param key the index of the key
 * @return the state of the keypad button
 */
bool keypad_keydown(int key);

/**
 * Returns true if the button was not down last frame, but is down this frame.
 * @param key the index of the key
 * @return true if the button was just pressed this very frame
 */
bool keypad_keypressed(int key);

/**
 * Returns true if the given button was released this frame
 * @param key the index of the key
 * @param was_down_frames an optional int to store how many frames the button was considered down
 * @return true if the button was down last frame, but is up this frame
 * If false is returned, was_down_frames is unchanged;
 */
bool keypad_keyreleased(int key, int* was_down_frames);
