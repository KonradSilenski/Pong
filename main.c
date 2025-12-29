#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "os_interface.h"
#include "g_loop.h"
#include "obj_data.h"
#include "config.h"
#include "audio.h"

int main (int argc, char **argv)
{
    os_SetWindowFrameBuffer(FRAME_WIDTH, FRAME_HEIGHT);

    os_Init();
    os_WindowSize (FRAME_WIDTH, FRAME_HEIGHT);

    InitSound();

    LoopInit();
    InitPlayer(FRAME_WIDTH, FRAME_HEIGHT);
    InitEnemy(FRAME_WIDTH, FRAME_HEIGHT);
    InitBall(FRAME_WIDTH, FRAME_HEIGHT);

    while(!sys_dat.win32.quit)
    {

        PongLoop();

        os_UpdateBuffer();
        os_NextEvent();
        os_DrawScreen();
    }

    TerminateSound();
    return 0;
}