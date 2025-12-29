#include <math.h>
#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "controls.h"
#include "os_interface.h"
#include "config.h"

void PlayerControls (Palette* player, int wnd_height)
{
    player->accel      = PLAYER_ACCEL;
    player->decel      = PLAYER_DECEL;
    player->moving_y   = 0;

    // Determine the direction of movement based on key input
    if (keyboard[VK_UP]    || keyboard['W']) { player->moving_y = 1; }
    if (keyboard[VK_DOWN]  || keyboard['S']) { player->moving_y = -1; }

    // Calculate movement velocity
    float target_speed = player->moving_y ? player->max_speed : 0.0f;

    player->speed += (target_speed - player->speed) * 0.15f;

    if (player->moving_y != 0)
        player->vel_y += player->moving_y * player->accel;
    else
        player->vel_y *= player->decel;

    if (player->vel_y >  player->max_speed) player->vel_y =  player->max_speed;
    if (player->vel_y < -player->max_speed) player->vel_y = -player->max_speed;

    // Move the player according to calculated velocity
    if (player->vel_y < 0)
    {
        player->y = ceil(player->y + player->vel_y);
    } else if (player->vel_y >= 0) {
        player->y += player->vel_y;
    }

    // Screen bounds collision checks
    if (player->y < 0) {player->y = 0; player->vel_y = 0.0f;};
    if (player->y > wnd_height - PADDLE_H*SCALING_VALUE) {player->y = wnd_height - PADDLE_H*SCALING_VALUE; player->vel_y = 0.0f;};
}