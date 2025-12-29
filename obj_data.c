#include "obj_data.h"

Palette player_dat = {0};
Palette enemy_dat = {0};
Ball ball_dat = {0};

void InitPlayer (int wnd_width, int wnd_height)
{
    player_dat.max_speed = PLAYER_MAX_SPD;
    player_dat.accel = PLAYER_ACCEL;
    player_dat.decel = PLAYER_DECEL;
    player_dat.x = PLAYER_START_X;
    player_dat.y = wnd_height / 2 - ((PADDLE_H/2)*SCALING_VALUE);
    player_dat.vel_y = 0.0f;
}

void InitEnemy (int wnd_width, int wnd_height)
{
    enemy_dat.max_speed = ENEMY_MAX_SPD;
    enemy_dat.accel = ENEMY_ACCEL;
    enemy_dat.decel = ENEMY_DECEL;
    enemy_dat.x = ENEMY_START_X(wnd_width);
    enemy_dat.y = wnd_height / 2 - ((PADDLE_H/2)*SCALING_VALUE);
    enemy_dat.vel_y = 0.0f;
}

void InitBall (int wnd_width, int wnd_height)
{
    ball_dat.x = BALL_START_X(wnd_width);
    ball_dat.y = BALL_START_Y(wnd_height);
    ball_dat.vel_x = BALL_VEL_X*BALL_SPEED;
    ball_dat.vel_y = BALL_VEL_Y*BALL_SPEED;
    ball_dat.restart = true;
    ball_dat.restart_tick = BALL_RESTART_TICK;
    ball_dat.speed = BALL_SPEED;
}