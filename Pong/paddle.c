// file: src/paddle.c
#include "paddle.h"

#include <allegro5/allegro_primitives.h>

#include "resources.h"

#define PADDLE_INIT_POS \
  (HEIGHT / 2) - (PADDLE_HEIGHT / 2) - (BOTTOM_PADDING - TOP_PADDING)

void paddle_init(paddle_t* paddle, paddle_side side) {
  paddle->side = side;
  paddle->position = PADDLE_INIT_POS;
}

void paddle_draw(paddle_t* paddle) {
  int x;
  if (paddle->side == left_player) {
    x = PADDLE_SPACING;
  } else {
    x = WIDTH - (PADDLE_WIDTH + PADDLE_SPACING);
  }
  int y = paddle->position;
  al_draw_filled_rectangle(x, y, x + PADDLE_WIDTH, y + PADDLE_HEIGHT, COLOR_FG,
                           0);
}

bool paddle_try_move(paddle_t* paddle, paddle_move_direction direction) {
  int new_position = paddle->position + direction;
  if (new_position >= TOP_PADDING + LINE_THICKNESS &&
      new_position + PADDLE_HEIGHT <=
          HEIGHT - (BOTTOM_PADDING + LINE_THICKNESS)) {
    paddle->position = new_position;
    return true;
  }
  return false;
}
