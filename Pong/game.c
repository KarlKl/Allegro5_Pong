// file: src/game.c
#include "game.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "paddle.h"
#include "resources.h"
#include "utils.h"

typedef struct {
  bool player1_up;
  bool player1_down;
  bool player2_up;
  bool player2_down;
} controls_t;
static controls_t active_controls;

static ball_t ball;
static bool game_paused = false;
static bool new_round = true;

static paddle_t paddle_left;
static paddle_t paddle_right;
static int points_left;
static int points_right;

void game_init(void) {
  srand(time(NULL));
  points_left = 0;
  points_right = 0;
  paddle_init(&paddle_left, left_player);
  paddle_init(&paddle_right, right_player);
  ball_init(&ball);
  al_set_display_icon(disp, icon);
}

void game_update(ALLEGRO_TIMER_EVENT event) {
  if (game_paused) {
    return;
  }
  if (active_controls.player1_up) {
    paddle_try_move(&paddle_left, direction_up);
  }
  if (active_controls.player1_down) {
    paddle_try_move(&paddle_left, direction_down);
  }
  if (active_controls.player2_up) {
    paddle_try_move(&paddle_right, direction_up);
  }
  if (active_controls.player2_down) {
    paddle_try_move(&paddle_right, direction_down);
  }
  int ball_out = ball_try_move(&ball);
  if (ball_out < 0) {
    points_left++;
    ball_reset(&ball, equal);
    new_round = true;
  } else if (ball_out > 0) {
    points_right++;
    ball_reset(&ball, equal);
    new_round = true;
  }
}

void game_redraw(void) {
  if (game_paused) {
    pause_draw();
    al_flip_display();
    return;
  }
  al_clear_to_color(COLOR_BG);
  score_draw();
  frame_draw();
  paddle_draw(&paddle_left);
  paddle_draw(&paddle_right);
  ball_draw(&ball);
  instructions_draw();
  al_flip_display();
}

void mouse_move(ALLEGRO_MOUSE_EVENT event) {}

void mouse_down(ALLEGRO_MOUSE_EVENT event) {}

void mouse_up(ALLEGRO_MOUSE_EVENT event) {}

void key_down(ALLEGRO_KEYBOARD_EVENT event) {
  switch (event.keycode) {
    case ALLEGRO_KEY_W:
      active_controls.player1_up = true;
      break;
    case ALLEGRO_KEY_S:
      active_controls.player1_down = true;
      break;
    case ALLEGRO_KEY_UP:
      active_controls.player2_up = true;
      break;
    case ALLEGRO_KEY_DOWN:
      active_controls.player2_down = true;
      break;
  }
}

static bool ball_movement_set(ball_movement_t *movement) {
  return movement->velocity_x != 0 && movement->velocity_y != 0;
}

static void handle_pause_game() {
  if (!game_paused && !new_round) {
    game_paused = true;
  } else {
    if (new_round) {
      // fresh start
      ball_movement_randomize(&ball.movement, equal);
      new_round = false;
    }
    game_paused = false;
  }
}

void key_up(ALLEGRO_KEYBOARD_EVENT event) {
  switch (event.keycode) {
    case ALLEGRO_KEY_W:
      active_controls.player1_up = false;
      break;
    case ALLEGRO_KEY_S:
      active_controls.player1_down = false;
      break;
    case ALLEGRO_KEY_UP:
      active_controls.player2_up = false;
      break;
    case ALLEGRO_KEY_DOWN:
      active_controls.player2_down = false;
      break;
    case ALLEGRO_KEY_SPACE:
      handle_pause_game();
      break;
  }
}

void key_char(ALLEGRO_KEYBOARD_EVENT event) {}

void score_draw() {
  int middle = WIDTH / 2;
  // points of left player
  al_draw_textf(font_big, COLOR_FG, middle - 20, 15, ALLEGRO_ALIGN_RIGHT, "%d",
                points_left);
  // points of right player
  al_draw_textf(font_big, COLOR_FG, middle + 29, 15, 0, "%d", points_right);
}

void draw_vertical_dashed_line(float x, float y, float y1, ALLEGRO_COLOR color,
                               float thickness, float dashsize) {
  float stepsize = dashsize * 1.75f;
  float overflow = stepsize - dashsize;
  for (float i = y; i < y1 - overflow; i += stepsize) {
    al_draw_filled_rectangle(x, i, x + thickness, i + dashsize, color);
  }
}

void frame_draw() {
  // top line
  al_draw_filled_rectangle(0, TOP_PADDING, WIDTH, TOP_PADDING + LINE_THICKNESS,
                           COLOR_FG);
  // bottom line
  al_draw_filled_rectangle(0, HEIGHT - BOTTOM_PADDING - LINE_THICKNESS, WIDTH,
                           HEIGHT - BOTTOM_PADDING, COLOR_FG);

  // draw dotted middle line
  draw_vertical_dashed_line(WIDTH / 2 - LINE_THICKNESS / 2, TOP_PADDING,
                            HEIGHT - BOTTOM_PADDING, COLOR_FG, LINE_THICKNESS,
                            2 * LINE_THICKNESS);
}

static bool bounding_box_intersect(int x1, int y1, int w1, int h1,  //
                                   int x2, int y2, int w2, int h2) {
  return (abs((x1 + w1 / 2) - (x2 + w2 / 2)) * 2 < (w1 + w2)) &&
         (abs((y1 + h1 / 2) - (y2 + h2 / 2)) * 2 < (h1 + h2));
}

int ball_try_move(ball_t *ball) {
  int new_x = ball->x + ball->movement.velocity_x;
  int new_y = ball->y + ball->movement.velocity_y;

  if (new_y <= TOP_PADDING + LINE_THICKNESS ||
      new_y >= HEIGHT - (BOTTOM_PADDING + LINE_THICKNESS)) {
    ball->movement.velocity_y *= -1;
    al_play_sample(audio_wall, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
    return ball_try_move(ball);
  }
  // calculate right upper edge of ball
  int ball_x_end = new_x + ball->size;
  int paddle_right_x = WIDTH - (PADDLE_WIDTH + PADDLE_SPACING);
  // check if ball enteres "critical" hitarea, then check boundingboxes
  if (new_x <= PADDLE_SPACING + PADDLE_WIDTH &&
      bounding_box_intersect(new_x, new_y, ball->size, ball->size,
                             PADDLE_SPACING, paddle_left.position, PADDLE_WIDTH,
                             PADDLE_HEIGHT)) {
    ball_movement_randomize(&ball->movement, right);
    al_play_sample(audio_paddle, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL); 
  } else if (ball_x_end > paddle_right_x &&
             bounding_box_intersect(new_x, new_y, ball->size, ball->size,
                                    paddle_right_x, paddle_right.position,
                                    PADDLE_WIDTH, PADDLE_HEIGHT)) {
    ball_movement_randomize(&ball->movement, left);
    al_play_sample(audio_paddle, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
  }
  if (ball_x_end <= 0) {
    al_play_sample(audio_lost, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
    return -1;
  } else if (new_x >= WIDTH) {
    al_play_sample(audio_lost, 1.0, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
    return 1;
  }
  ball->x = new_x;
  ball->y = new_y;
  return 0;
}

void pause_draw() {
  al_draw_text(font_big, COLOR_FG, WIDTH / 2, HEIGHT / 2 - FONT_HEIGHT_BIG,
               ALLEGRO_ALIGN_CENTRE, "PAUSE");
}

void instructions_draw() {
  al_draw_text(font_small, COLOR_FG, WIDTH, HEIGHT - BOTTOM_PADDING + LINE_THICKNESS,
               ALLEGRO_ALIGN_RIGHT, "W/S  -  LEFT PLAYER   ~   UP/DOWN - RIGHT PLAYER   ~   SPACE - START / PAUSE   ");
}
