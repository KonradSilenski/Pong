#ifndef __OBJ_DAT__
#define __OBJ_DAT__

#include "config.h"
#include <stdbool.h>

typedef struct {
    int x;
    int y;
    float accel;
    float decel;
    float vel_y;
    int moving_y;
    int speed;
    int max_speed;
    int score;
} Palette;

typedef struct {
    int x;
    int y;
    float vel_x;
    float vel_y;
    bool restart;
    int restart_tick;
    int speed;
} Ball;

extern Palette player_dat;
extern Palette enemy_dat;
extern Ball ball_dat;

void InitPlayer (int wnd_width, int wnd_height);    // Initialises or resets player values.
void InitEnemy (int wnd_width, int wnd_height);     // Initialises or resets enemy values.
void InitBall (int wnd_width, int wnd_height);      // Initialises or resets ball values.

#endif