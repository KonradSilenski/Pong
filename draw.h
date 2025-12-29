#ifndef __DRAW__
#define __DRAW__

#include <stdint.h>
#include <stdbool.h>
#include "config.h"

/* 
    Draws the paddle bitmap to the buffer at specified coordinates. 
    Uses SCALING_VALUE to scale the bitmap.
*/
void draw_paddle(
    uint32_t *buffer,
    int wind_width,
    int wind_height,
    int x,
    int y,
    uint32_t color
);

/*
    Draws the ball bitmap to the buffer at specified coordinates.
    Uses SCALING_VALUE to scale the bitmap and centers the image.
*/
void draw_ball(
    uint32_t *buffer,
    int wind_width,
    int wind_height,
    int x,
    int y,
    uint32_t color
);

void draw_digit(
    uint32_t *pixels,
    int width,
    int height,
    int x,
    int y,
    int digit,
    uint32_t color
);

void draw_number(
    uint32_t *pixels,
    int width,
    int height,
    int x,
    int y,
    int number,
    uint32_t color
);

void DrawNet(
    uint32_t *pixels,
    int width,
    int height,
    uint32_t color
);

#endif