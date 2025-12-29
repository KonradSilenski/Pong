#ifndef __CONF__
#define __CONF__

// Window size
#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480

// Player initialisation
#define PLAYER_START_X 50
#define PLAYER_ACCEL 0.5f
#define PLAYER_DECEL 0.85f
#define PLAYER_MAX_SPD 10

// Enemy initialisation
#define ENEMY_START_X(w) w-55
#define ENEMY_ACCEL 0.5f
#define ENEMY_DECEL 0.85f
#define ENEMY_MAX_SPD 10

// Ball initialisation
#define BALL_START_X(w) (w/2)
#define BALL_START_Y(h) (h/2)
#define BALL_VEL_X -1
#define BALL_VEL_Y 0
#define BALL_RESTART_TICK 60
#define BALL_SPEED 5

// Game loop initialisation
#define TICKSPERSEC 60
#define SKIPTICKS 1000
#define FRAMESKIP 5

// Bitmap scaling power
#define SCALING_VALUE 4

// Game font size
#define FONT_W 5
#define FONT_H 7

// Paddle size
#define PADDLE_W 1
#define PADDLE_H 12

// Ball size
#define BALL_W 2
#define BALL_H 2

// Net number
#define NET_ITERATOR (FRAME_HEIGHT/PADDLE_H)

#endif