#include "fpga_spi.h"
#include <stdio.h>
#include "sim_config.h"
#include "raylib.h"

// Whenever a byte is sent, we want a light to flash for a few frames
static int usage_light = 0;
#define BLINK_TIME 7
static int total_bytes_sent = 0;

void fpga_spi_init() {

}

void fpga_spi_send(uint8_t* data, uint8_t len) {
    fprintf(stderr, "Sent to FPGA:");
    for(int i = 0; i < len; i++)
        fprintf(stderr, " 0x%2X", data[i]);
    fprintf(stderr, "\n");
    usage_light = BLINK_TIME;
    total_bytes_sent += len;
}

void fpga_spi_draw() {
    BeginMode2D((Camera2D){{FPGA_SPI_X, FPGA_SPI_Y}, .zoom=1.0f});
    Color c = BEIGE;
    if (usage_light) {
        c = YELLOW;
        usage_light--;
    }
    DrawRectangle(0, 0, 20, 20, c);
    DrawText("FPGA SPI bus", 25, 0, 20, BLACK);
    DrawText(TextFormat("Total bytes sent: %d", total_bytes_sent), 0, 25, 20, BLACK);
    EndMode2D();
}
