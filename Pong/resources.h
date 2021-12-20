// file: src/resources.h

#pragma once

#include <stdbool.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>

#define WIDTH 800
#define HEIGHT 500
#define FONT_SIZE_BIG 90
#define FONT_HEIGHT_BIG FONT_SIZE_BIG / 2
#define FONT_SIZE_SMALL 18
#define FPS 60
#define REFRESH_RATE (1.0 / FPS)
#define COLOR_BG al_map_rgb(0, 0, 0)
#define COLOR_FG al_map_rgb(255, 255, 255)

#define TOP_PADDING 10
#define BOTTOM_PADDING 30
#define LINE_THICKNESS 5

extern bool done;
extern bool redraw;

extern ALLEGRO_TIMER *timer;
extern ALLEGRO_EVENT_QUEUE *queue;
extern ALLEGRO_DISPLAY *disp;
extern ALLEGRO_BITMAP *icon;
extern ALLEGRO_FONT *font_big;
extern ALLEGRO_FONT *font_small;
extern ALLEGRO_SAMPLE *audio_wall;
extern ALLEGRO_SAMPLE *audio_paddle;
extern ALLEGRO_SAMPLE *audio_lost;

void init_assets(void);
void destroy_assets(void);

void register_events(void);
void unregister_events(void);