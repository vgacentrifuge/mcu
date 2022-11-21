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

#define KEY_PSTATE KEY_INDEX(0,0)
#define KEY_NSTATE  KEY_INDEX(1,0)

#define KEY_TRANSPLUS  KEY_INDEX(2,0)
#define KEY_TRANSMINUS KEY_INDEX(2,1)

#define KEY_SCALEUP  KEY_INDEX(3,0)
#define KEY_SCALEDOWN KEY_INDEX(3,1)

#define KEY_CHROMA  KEY_INDEX(0,1)
#define KEY_OVERLAY KEY_INDEX(0,2)
#define KEY_NONE    KEY_INDEX(0,3)

#define KEY_RESET_ALL KEY_INDEX(1,1)
#define KEY_RESET_OFFSET KEY_INDEX(1,2)
#define KEY_MENU      KEY_INDEX(3,2)

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
 * @param was_down_frames an optional place to output the press duration (starting at 1 on press frame)
 * @return the state of the keypad button
 */
bool keypad_keydown(int key, int* was_down_frames);

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
