#ifndef __ENEMLOG__
#define __ENEMLOG__

#include "config.h"
#include "obj_data.h"

/*
    Calculates the movement of the enemy palette based on the coordinates
    of the ball, and performs collision checks with the edges of the window.
*/
void EnemyLogic(int width, int height, Ball* ball, Palette* ai_player);

#endif