#ifndef __G_LOOP__
#define __G_LOOP__

#include <windows.h>
#include <stdbool.h>
#include "config.h"

typedef struct {
    int ticks_per_second;
    int skip_ticks;
    int max_frameskip;
    int tick;
    int loops;
    DWORD next_game_tick;
} Loop_Data;

extern Loop_Data loop_dat;

// Runs the game logic and user input loop at a specified rate per second.
void PongLoop (void);

// Initialises the game loop values.
void LoopInit (void);

#endif