#include "raylib.h"
const int RAYLIB_KEY_UP = KEY_UP;
const int RAYLIB_KEY_DOWN = KEY_DOWN;
const int RAYLIB_KEY_LEFT = KEY_LEFT;
const int RAYLIB_KEY_RIGHT = KEY_RIGHT;
#include "keypad.h"
#include <string.h>

#include "sim_config.h"

#define KEY_SPACING 5
#define KEY_WIDTH ((KEYPAD_WIDTH-KEY_SPACING*(KEYPAD_COLS-1)) / KEYPAD_COLS)
#define KEY_HEIGHT ((KEYPAD_HEIGHT-KEY_SPACING*(KEYPAD_ROWS-1)) / KEYPAD_ROWS)
#define KEY_STRIDE_X (KEY_WIDTH+KEY_SPACING)
#define KEY_STRIDE_Y (KEY_HEIGHT+KEY_SPACING)

bool previousState[KEYPAD_ROWS * KEYPAD_COLS];
bool currentState[KEYPAD_ROWS * KEYPAD_COLS];

const char* keySymbols[KEYPAD_ROWS * KEYPAD_COLS] = {
[KEY_UP] = "^",
[KEY_LEFT] = "<",
[KEY_RIGHT] = ">",
[KEY_DOWN] = "V"
};

const int keyboardAlternatives[KEYPAD_ROWS * KEYPAD_COLS] = {
[KEY_UP] = RAYLIB_KEY_UP,
[KEY_LEFT] = RAYLIB_KEY_LEFT,
[KEY_RIGHT] = RAYLIB_KEY_RIGHT,
[KEY_DOWN] = RAYLIB_KEY_DOWN
};

void keypad_init() {

}

void keypad_sample() {
    // Store the previous state, to get exactly one frame of pressing and releasing
    memcpy(previousState, currentState, sizeof(currentState));

    int mouseX = GetMouseX() - KEYPAD_X;
    int mouseY = GetMouseY() - KEYPAD_Y;
    bool mouseDown = IsMouseButtonDown(0);
    bool mousePressed = IsMouseButtonPressed(0);
    for(int y = 0; y < KEYPAD_ROWS; y++) {
        for(int x = 0; x < KEYPAD_COLS; x++) {
            int localX = mouseX - x*KEY_STRIDE_X;
            int localY = mouseY - y*KEY_STRIDE_Y;
            bool hover = localX >= 0 && localX < KEY_WIDTH && localY >= 0 && localY < KEY_HEIGHT;
            int index = KEY_INDEX(x, y);
            currentState[index] = hover && (mousePressed || (mouseDown && previousState[index]));

            if(keyboardAlternatives[index] && IsKeyDown(keyboardAlternatives[index]))
                currentState[index] = true;
        }
    }
}

bool keypad_keydown(int key) {
    return currentState[key];
}

bool keypad_keypressed(int key) {
    return !previousState[key] && currentState[key];
}

bool keypad_keyreleased(int key) {
    return previousState[key] && !currentState[key];
}

void keypad_draw() {
    BeginMode2D((Camera2D){{KEYPAD_X, KEYPAD_Y}, .zoom=1.0f});
    for(int y = 0; y < KEYPAD_COLS; y++) {
        for(int x = 0; x < KEYPAD_ROWS; x++) {
            int index = KEY_INDEX(x, y);
            Color color = currentState[index] ? BLACK : LIGHTGRAY;
            DrawRectangle(x*KEY_STRIDE_X, y*KEY_STRIDE_Y, KEY_WIDTH, KEY_HEIGHT, color);
            if(keySymbols[index])
                DrawText(keySymbols[index], x*KEY_STRIDE_X, y*KEY_STRIDE_Y, KEY_HEIGHT, GRAY);
        }
    }
    EndMode2D();
}
