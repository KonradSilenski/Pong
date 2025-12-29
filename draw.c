#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "draw.h"
#include "bitmaps.h"

void draw_paddle(
    uint32_t *pixels,
    int wind_width,
    int wind_height,
    int x,
    int y,
    uint32_t color
) {
    int scale = SCALING_VALUE;
    for (int row = 0; row < PADDLE_H; row++) {
        uint8_t bits = paddle_btmp[row];

        for (int col = 0; col < PADDLE_W; col++) {
            if (bits & (1 << (PADDLE_W - 1 - col))) {

                int base_x = x + col * scale;
                int base_y = y + row * scale;

                for (int sy = 0; sy < scale; sy++) {
                    int py = base_y + sy;
                    if (py < 0 || py >= wind_height) continue;

                    for (int sx = 0; sx < scale; sx++) {
                        int px = base_x + sx;
                        if (px < 0 || px >= wind_width) continue;
                        
                        pixels[py * wind_width + px] = color;
                    }
                }
            }
        }
    }
};

void draw_ball(
    uint32_t *pixels,
    int wind_width,
    int wind_height,
    int x,
    int y,
    uint32_t color
) {
    x -= BALL_W;
    y -= BALL_H;
    int scale = SCALING_VALUE-1;
    for (int row = 0; row < BALL_H; row++) {
        uint8_t bits = paddle_btmp[row];

        for (int col = 0; col < BALL_W; col++) {
            if (bits & (1 << (BALL_W - 1 - col))) {

                int base_x = x + col * scale;
                int base_y = y + row * scale;

                for (int sy = 0; sy < scale; sy++) {
                    int py = base_y + sy;
                    if (py < 0 || py >= wind_height) continue;

                    for (int sx = 0; sx < scale; sx++) {
                        int px = base_x + sx;
                        if (px < 0 || px >= wind_width) continue;
                        
                        pixels[py * wind_width + px] = color;
                    }
                }
            }
        }
    }
};

void draw_digit(
    uint32_t *pixels,
    int width,
    int height,
    int x,
    int y,
    int digit,
    uint32_t color
) {
    int scale = SCALING_VALUE;
    for (int row = 0; row < FONT_H; row++) {
        uint8_t bits = digit_font[digit][row];

        for (int col = 0; col < FONT_W; col++) {
            if (bits & (1 << (FONT_W - 1 - col))) {

                int base_x = x + col * scale;
                int base_y = y + row * scale;

                for (int sy = 0; sy < scale; sy++) {
                    int py = base_y + sy;
                    if (py < 0 || py >= height) continue;

                    for (int sx = 0; sx < scale; sx++) {
                        int px = base_x + sx;
                        if (px < 0 || px >= width) continue;

                        pixels[py * width + px] = color;
                    }
                }
            }
        }
    }
};

void draw_number(
    uint32_t *pixels,
    int width,
    int height,
    int x,
    int y,
    int number,
    uint32_t color
) {
    char buf[16];
    snprintf(buf, sizeof buf, "%d", number);

    for (int i = 0; buf[i]; i++) {
        draw_digit(
            pixels,
            width,
            height,
            x + i * (24),
            y,
            buf[i] - '0',
            color
        );
    }
};

void DrawNet(
    uint32_t *pixels,
    int width,
    int height,
    uint32_t color
)
{
    int scale = 1;
    int x = width/2;
    for (int i = 0; i < NET_ITERATOR; ++i)
    {
        int y = (i*2)*PADDLE_H;
        for (int row = 0; row < PADDLE_H; row++) {
            uint8_t bits = paddle_btmp[row];

            for (int col = 0; col < PADDLE_W; col++) {
                if (bits & (1 << (PADDLE_W - 1 - col))) {

                    int base_x = x + col * scale;
                    int base_y = y + row * scale;

                    for (int sy = 0; sy < scale; sy++) {
                        int py = base_y + sy;
                        if (py < 0 || py >= height) continue;

                        for (int sx = 0; sx < scale; sx++) {
                            int px = base_x + sx;
                            if (px < 0 || px >= width) continue;
                            
                            pixels[py * width + px] = color;
                    }
                }
            }
        }
    }
    }
}