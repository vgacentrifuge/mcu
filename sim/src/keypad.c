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

int key_down_time[KEYPAD_ROWS * KEYPAD_COLS];

const char* keySymbols[KEYPAD_ROWS * KEYPAD_COLS] = {
[KEY_UP] = "^",
[KEY_LEFT] = "<",
[KEY_RIGHT] = ">",
[KEY_DOWN] = "V",

[PSTATE_KEY] = "Ps",
[NSTATE_KEY] = "Ns",

[TRANSPLUS_KEY] = "+T",
[TRANSMINUS_KEY] = "-T",
[SCALEUP_KEY] = "+S",
[SCALEDOWN_KEY] = "-S",

[CHROMA_KEY] = "CHR\nOMA",
[OVERLAY_KEY] = "OVR\nLAY",
[NONE_KEY] = "NON",

[RESET_ALL_KEY] = "Rst",
[RESET_OFFSET_KEY] = ">:<",
[MENU_KEY] = "MNU"
};
#define KEY_TEXT_SIZE 40

const int keyboardAlternatives[KEYPAD_ROWS * KEYPAD_COLS] = {
[KEY_UP] = RAYLIB_KEY_UP,
[KEY_LEFT] = RAYLIB_KEY_LEFT,
[KEY_RIGHT] = RAYLIB_KEY_RIGHT,
[KEY_DOWN] = RAYLIB_KEY_DOWN
};

void keypad_init() {

}

void keypad_sample() {
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

            // Just like in the actual keypad code,
            // a negative down time means the last frame was a release
            if (key_down_time[index] < 0)
                key_down_time[index] = 0;

            bool startingPress = hover && mousePressed;
            bool holdingPress = key_down_time[index] >= 1 && hover && mouseDown;
            bool keyPressing = keyboardAlternatives[index] && IsKeyDown(keyboardAlternatives[index]);

            if (startingPress || holdingPress || keyPressing)
                key_down_time[index]++;
            else if(key_down_time[index] > 0) // We just released the key
                key_down_time[index] = -key_down_time[index];
        }
    }
}

bool keypad_keydown(int key, int* was_down_frames) {
    bool down = key_down_time[key]>=1;
    if (down && was_down_frames)
        *was_down_frames = key_down_time[key];
    return down;
}

bool keypad_keypressed(int key) {
    return key_down_time[key]==1;
}

bool keypad_keyreleased(int key, int* was_down_frames) {
    bool released = key_down_time[key] < 0;
    if (released && was_down_frames)
        *was_down_frames = -key_down_time[key];
    return released;
}

void keypad_draw() {
    BeginMode2D((Camera2D){{KEYPAD_X, KEYPAD_Y}, .zoom=1.0f});
    for(int y = 0; y < KEYPAD_COLS; y++) {
        for(int x = 0; x < KEYPAD_ROWS; x++) {
            int index = KEY_INDEX(x, y);
            Color color = keypad_keydown(index, NULL) ? BLACK : LIGHTGRAY;
            DrawRectangle(x*KEY_STRIDE_X, y*KEY_STRIDE_Y, KEY_WIDTH, KEY_HEIGHT, color);
            if(keySymbols[index])
                DrawText(keySymbols[index], x*KEY_STRIDE_X, y*KEY_STRIDE_Y, KEY_TEXT_SIZE, GRAY);
        }
    }
    EndMode2D();
}
