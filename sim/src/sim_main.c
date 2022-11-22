/*
 * sim_main.c
 *
 *  Created on: Oct 16, 2022
 *      Author: havard
 */

#include "raylib.h"
#include "app.h"
#include "sim_config.h"

void board_io_draw();
void lcd_draw();
void keypad_draw();
void fpga_spi_draw();

int main(int argc, char **argv) {
    (void) argc; (void) argv;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MCU simulator");
    SetTargetFPS(60);

    app_init();

    while(!WindowShouldClose()) {
        app_process_action();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        lcd_draw();
        board_io_draw();
        keypad_draw();
        fpga_spi_draw();

        EndDrawing();
    }

    CloseWindow();
}
