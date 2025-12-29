#ifndef __CTRLS__
#define __CTRLS__

#include <stdbool.h>
#include "obj_data.h"

/*
Checks for key-specific input from the player and calculates
their movement exponentially. Also checks for collisions with
upper and lower bounds of the screen.
*/
void PlayerControls (Palette* player, int wnd_height);

#endif