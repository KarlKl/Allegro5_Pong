// file: src/ball.h

#pragma once

#include <stdbool.h>

#define BALL_SIZE 15

#define VELOCITY_X_MAX 7
#define VELOCITY_X_MIN 5
#define VELOCITY_Y_MAX 8
#define VELOCITY_Y_MIN 4

typedef struct {
  float velocity_x;
  float velocity_y;
} ball_movement_t;

typedef enum { left, right, equal } ball_movement_direction;

typedef struct {
  int size;
  int x;
  int y;
  ball_movement_t movement;
} ball_t;

void ball_init(ball_t *ball);
void ball_reset(ball_t *ball, ball_movement_direction direction);
void ball_movement_randomize(ball_movement_t *movement,
                             ball_movement_direction direction);

void ball_draw(ball_t *ball);
