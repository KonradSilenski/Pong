#include "ball_logic.h"
#include "obj_data.h"
#include "config.h"
#include "audio.h"
#include "sound_tracks.h"

void BallLogic(int width, int height, Ball* ball, Palette* player, Palette* ai_player)
{
    // Top and bottom edge collision
    if (ball->y <= 0 || ball->y + ball->vel_y - height >= 0)
    {
        ball->vel_y *= -1;
        PlayBeep(BEEP);
    }

    // Right and left edge collision
    if (ball->x < 0 || ball->x -width > 0)
    {
        if (ball->x < 0)
        {
            ++ai_player->score;
            PlayBeep(ENEMY_GOAL);
        }
        if (ball->x - width > 0)
        {
            ++player->score;
            PlayBeep(PLAYER_GOAL);
        }
        
        InitBall(width, height);
        player->moving_y = 0;
        player->vel_y = 0.0f;
        ai_player->moving_y = 0;
        ai_player->vel_y = 0.0f;
    }
    
    // Check for player pallette collision
    if ((ball->x + ball->vel_x) < player->x + (PADDLE_W*SCALING_VALUE) &&
        (ball->x + ball->vel_x) >= player->x &&
        ((ball->y-BALL_H) + ball->vel_y) < player->y + PADDLE_H*SCALING_VALUE &&
        ((ball->y+BALL_H) + ball->vel_y) > player->y){
            
            PlayBeep(BEEP);
            int diff = ball->y - (player->y + PADDLE_H*2);
            float perc = (float)diff/(PADDLE_H*2);
            ball->vel_y = perc*ball->speed;
            ball->vel_x *= -1;
        }
    
    // Check for enemy pallette collision
    if ((ball->x + ball->vel_x) >= ai_player->x &&
        (ball->x + ball->vel_x) < ai_player->x + (PADDLE_W*SCALING_VALUE) &&
        ((ball->y+BALL_H) + ball->vel_y) < ai_player->y + PADDLE_H*SCALING_VALUE &&
        ((ball->y+BALL_H) + ball->vel_y) > ai_player->y){

            PlayBeep(BEEP);
            int diff = ball->y - (ai_player->y + PADDLE_H*2);
            float perc = (float)diff/(PADDLE_H*2);
            ball->vel_y = perc*ball->speed;
            ball->vel_x *= -1;
        }
    
    // Move the ball or decrement the restart timer
    if (!ball->restart)
        {
            ball->x += ball->vel_x;
            ball->y += ball->vel_y;
        } else {
            ball->restart_tick -= 1;
            if (ball->restart_tick == 0) ball->restart = false;
        }
}