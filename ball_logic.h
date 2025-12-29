#ifndef __BALLLOG__
#define __BALLLOG__

#include "config.h"
#include "obj_data.h"

// Performs collision checks for the ball and moves it
void BallLogic(int width, int height, Ball* ball, Palette* player, Palette* ai_player);

#endif