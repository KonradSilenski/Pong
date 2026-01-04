#include "g_loop.h"
#include "controls.h"
#include "obj_data.h"
#include "ball_logic.h"
#include "enemy_logic.h"
#include "config.h"
#include "os_interface.h"

Loop_Data loop_dat = {0};

void PongLoop (void)
{
    loop_dat.loops = 0;
    while (GetTickCount() > loop_dat.next_game_tick && loop_dat.loops < loop_dat.max_frameskip)
    {
        // Only run game logic if the restart timer is not running.
        if (!pause){
            // Check for player input and perform movement calculations.
            if (!ball_dat.restart) PlayerControls(&player_dat, 480);
            // Perform in game object logic.
            BallLogic(FRAME_WIDTH, FRAME_HEIGHT, &ball_dat, &player_dat, &enemy_dat);
            if (!ball_dat.restart) EnemyLogic(FRAME_WIDTH, FRAME_HEIGHT, &ball_dat, &enemy_dat);
        }

        loop_dat.next_game_tick += loop_dat.skip_ticks;
        ++loop_dat.loops;
        if (loop_dat.tick < 60) ++loop_dat.tick; else loop_dat.tick = 0;
    }
}

void LoopInit (void)
{
    loop_dat.ticks_per_second = TICKSPERSEC;
    loop_dat.skip_ticks = SKIPTICKS / TICKSPERSEC;
    loop_dat.max_frameskip = FRAMESKIP;
    loop_dat.tick = 0;
    loop_dat.next_game_tick = GetTickCount();
}