#include "board_io.h"
#include "raylib.h"
#include "sim_config.h"

static bool led0, led1;
static bool button0, button1;

void board_io_init() {}

void board_io_sample() {
    int mouseX = GetMouseX() - BOARD_IO_X;
    int mouseY = GetMouseY() - BOARD_IO_Y;
    bool mousedown = IsMouseButtonDown(0);

    button0 = false;
    button1 = false;
    if(mouseY < 0 || mouseY >= BOARD_IO_HEIGHT)
        return;

    button0 = mouseX >= 130 && mouseX < 130+10 && mousedown;
    button1 = mouseX >= 145 && mouseX < 145+10 && mousedown;
}

bool board_io_get_button0() {
    return button0;
}

bool board_io_get_button1() {
    return button1;
}

void board_io_set_led0(bool state) {
    led0 = state;
}

void board_io_set_led1(bool state) {
    led1 = state;
}

void board_io_draw() {
    BeginMode2D((Camera2D){{BOARD_IO_X, BOARD_IO_Y}, .zoom=1.0f});
    DrawRectangle(0, 0, 10, BOARD_IO_HEIGHT, led0 ? YELLOW : BEIGE);
    DrawRectangle(15, 0, 10, BOARD_IO_HEIGHT, led1 ? YELLOW : BEIGE);

    DrawText("led0 led1   pb0 pb1", 30, 0, BOARD_IO_HEIGHT, GRAY);

    DrawRectangle(130, 0, 10, BOARD_IO_HEIGHT, board_io_get_button0() ? BLACK : GRAY);
    DrawRectangle(145, 0, 10, BOARD_IO_HEIGHT, board_io_get_button1() ? BLACK : GRAY);
    EndMode2D();
}
