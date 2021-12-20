// file: src/ball.c
#include "ball.h"

#include <allegro5/allegro_primitives.h>

#include "resources.h"
#include "utils.h"

void ball_init(ball_t *ball) {
  ball->size = BALL_SIZE;
  ball_reset(ball, equal);
}

void ball_reset(ball_t *ball) {
  ball->x = WIDTH / 2;
  ball->y = HEIGHT / 2;
  ball->movement.velocity_x = 0;
  ball->movement.velocity_y = 0;
}

void ball_movement_randomize(ball_movement_t *ball_movement,
                             ball_movement_direction direction) {
  float vel_x =
      ((float)rand() / RAND_MAX) * (VELOCITY_X_MAX * 2) - VELOCITY_X_MAX;
  if (is_within(vel_x, -VELOCITY_X_MIN, 0)) {
    vel_x = -VELOCITY_X_MIN;
  } else if (is_within(vel_x, 0, VELOCITY_X_MIN)) {
    vel_x = VELOCITY_X_MIN;
  }

  float vel_y =
      ((float)rand() / RAND_MAX) * (VELOCITY_Y_MAX * 2) - VELOCITY_Y_MAX;
  if (is_within(vel_y, -VELOCITY_Y_MIN, 0)) {
    vel_y = -VELOCITY_Y_MIN;
  } else if (is_within(vel_y, 0, VELOCITY_Y_MIN)) {
    vel_y = VELOCITY_Y_MIN;
  }
  switch (direction) {
    case left:
      vel_x = -abs(vel_x);
      vel_y = -abs(vel_y);
      break;
    case right:
      vel_x = abs(vel_x);
      vel_y = abs(vel_y);
      break;
  }
  ball_movement->velocity_x = vel_x;
  ball_movement->velocity_y = vel_y;
}

void ball_draw(ball_t *ball) {
  int half_size = ball->size / 2;
  int x1 = ball->x - half_size;
  int y1 = ball->y - half_size;
  int x2 = ball->x + half_size;
  int y2 = ball->y + half_size;
  al_draw_filled_rectangle(x1, y1, x2, y2, COLOR_FG);
}
