#ifndef PONG_H
#define PONG_H

#include <stdint.h>

#define BALL_SIZE       5
#define PADDLE_W        3
#define PADDLE_H        48
#define BALL_SPEED      5
#define MAX_BALL_SPEED  10
#define PADDLE_SPEED    3


typedef struct
{
    // left paddle pos
    int16_t l_x;
    int16_t l_y;

    // right paddle pos
    int16_t r_x;
    int16_t r_y;

    // ball pos
    int16_t b_x;
    int16_t b_y;
} pong_state_t;


void pong_init(void);

void pong_play(void);

#endif
