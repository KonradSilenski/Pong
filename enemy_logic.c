#include <math.h>
#include "enemy_logic.h"

void EnemyLogic(int width, int height, Ball* ball, Palette* ai_player)
{
    // Reset movement values on each new frame
    ai_player->accel      = ENEMY_ACCEL;
    ai_player->decel      = ENEMY_DECEL;
    ai_player->moving_y   = 0;

    // Determine direction based on the ball's position
    if (ball->y > ai_player->y + (PADDLE_H*2+6) && ball->x > width/2 - 150) {
        ai_player->moving_y = 1;
    }
    else if (ball->y < (ai_player->y + 6) && ball->x > width/2 - 150) {
        ai_player->moving_y = -1;
    }

    // Calculate movement velocity
    float target_speed = ai_player->moving_y ? ai_player->max_speed : 0.0f;

    ai_player->speed += (target_speed - ai_player->speed) * 0.15f;

    if (ai_player->moving_y != 0)
    {
        if (ai_player->moving_y == 1 && ai_player->vel_y < 0.0f)
        {
            ai_player->vel_y *= ai_player->decel;
        }
        if (ai_player->moving_y ==-1 && ai_player->vel_y > 0.0f)
        {
            ai_player->vel_y *= ai_player->decel;
        }
        ai_player->vel_y += ai_player->moving_y * ai_player->accel;
    }
    else
        ai_player->vel_y *= ai_player->decel;

    if (ai_player->vel_y >  ai_player->max_speed) ai_player->vel_y =  ai_player->max_speed;
    if (ai_player->vel_y < -ai_player->max_speed) ai_player->vel_y = -ai_player->max_speed;
    
    // Move the enemy palette according to the calculated velocity
    if (ai_player->vel_y < 0)
    {
        ai_player->y = ceil(ai_player->y + ai_player->vel_y);
    } else if (ai_player->vel_y >= 0) {
        ai_player->y += ai_player->vel_y;
    }

    // Perform collision checks with the edges of the window
    if (ai_player->y < 0) {ai_player->y = 0; ai_player->vel_y = 0.0f; ai_player->moving_y = 0;};
    if (ai_player->y > height - (PADDLE_H*SCALING_VALUE)) {ai_player->y = height - (PADDLE_H*SCALING_VALUE); ai_player->vel_y = 0.0f; ai_player->moving_y = 0;};
}