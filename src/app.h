#include <pebble.h>

#ifndef app_h
#define app_h

#define SCREEN_WIDTH 144
#define SCREEN_HEIGHT 168

extern Window *window;
extern TextLayer *text_layer;

void init_app(void);

void tick_every_second(struct tm *tick_time, TimeUnits units_changed);

void print_time(int hour, int min, int sec);

#endif

