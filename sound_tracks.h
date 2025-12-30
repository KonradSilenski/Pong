#ifndef __SNDTRCK__
#define __SNDTRCK__

#include <stdint.h>

#define LONG_TRACK_L 5
#define BEEP_L 2

static const float player_goal[2][LONG_TRACK_L] = 
{
    {450.0f, 0.0f, 450.0f, 0.0f, 700.0f},
    {0.1f, 0.1f, 0.1f, 0.1f, 0.1f}
};

static const float enemy_goal[2][LONG_TRACK_L] = 
{
    {450.0f, 0.0f, 450.0f, 0.0f, 200.0f},
    {0.1f, 0.1f, 0.1f, 0.1f, 0.1f}
};

static const float contact[2][BEEP_L] = 
{
    {200.0f, 450.0f},
    {0.05f, 0.1f}
};

typedef enum{
    PLAYER_GOAL,
    ENEMY_GOAL,
    BEEP
} TrackType;

#endif