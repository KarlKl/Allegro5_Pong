// file: src/paddle.h

#pragma once

#include <stdbool.h>

#define PADDLE_HEIGHT HEIGHT / 5
#define PADDLE_WIDTH 10
#define PADDLE_SPACING (2 * PADDLE_WIDTH)
#define SPEED 6

typedef enum { left_player, right_player } paddle_side;
typedef enum {
  direction_up = -SPEED,
  direction_down = SPEED
} paddle_move_direction;

typedef struct {
  paddle_side side;
  int position;
} paddle_t;

void paddle_init(paddle_t *paddle, paddle_side side);

void paddle_draw(paddle_t *paddle);
bool paddle_try_move(paddle_t *paddle, paddle_move_direction direction);
