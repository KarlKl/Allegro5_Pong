// file: src/resources.c
#include "resources.h"

#include "utils.h"

bool done = false;
bool redraw = true;

ALLEGRO_TIMER *timer;
ALLEGRO_EVENT_QUEUE *queue;
ALLEGRO_DISPLAY *disp;
ALLEGRO_BITMAP *icon;
ALLEGRO_FONT *font_big;
ALLEGRO_FONT *font_small;
ALLEGRO_SAMPLE *audio_wall;
ALLEGRO_SAMPLE *audio_paddle;
ALLEGRO_SAMPLE *audio_lost;

void init_assets(void) {
  timer = assert_not_null(al_create_timer(REFRESH_RATE), "timer");
  queue = assert_not_null(al_create_event_queue(), "event queue");
  disp = assert_not_null(al_create_display(WIDTH, HEIGHT), "display");
  icon = assert_not_null(al_load_bitmap("assets/icon.png"), "icon");
  font_small = assert_not_null(
      al_load_ttf_font("assets/E4_2017.ttf", FONT_SIZE_SMALL, 0), "font small");
  font_big = assert_not_null(
      al_load_ttf_font("assets/E4_2017.ttf", FONT_SIZE_BIG, 0), "font big");
  assert_true(al_reserve_samples(1), "default mixer");
  audio_wall = assert_not_null(al_load_sample("assets/wall.wav"), "audio wall");
  audio_paddle =
      assert_not_null(al_load_sample("assets/paddle.wav"), "audio paddle");
  audio_lost = assert_not_null(al_load_sample("assets/lost.wav"), "audio lost");
}

void destroy_assets(void) {
  al_destroy_bitmap(icon);
  al_destroy_font(font_big);
  al_destroy_font(font_small);
  al_destroy_sample(audio_wall);
  al_destroy_sample(audio_paddle);
  al_destroy_sample(audio_lost);
  al_destroy_display(disp);
  al_destroy_event_queue(queue);
  al_destroy_timer(timer);
}

void register_events(void) {
  al_register_event_source(queue, al_get_timer_event_source(timer));
  al_register_event_source(queue, al_get_keyboard_event_source());
  al_register_event_source(queue, al_get_mouse_event_source());
  al_register_event_source(queue, al_get_display_event_source(disp));
}

void unregister_events(void) {
  al_unregister_event_source(queue, al_get_display_event_source(disp));
  al_unregister_event_source(queue, al_get_mouse_event_source());
  al_unregister_event_source(queue, al_get_keyboard_event_source());
  al_unregister_event_source(queue, al_get_timer_event_source(timer));
}