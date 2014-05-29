#include <pebble.h>

#ifndef renderer_h
#define renderer_h

extern Window *window;

extern TextLayer *day_text_layer;
extern TextLayer *battery_text_layer;
extern Layer *second_display_layer;
extern Layer *minute_display_layer;
extern Layer *hour_display_layer;

extern GPath *hour_divider_segment_path;
extern GPath *hour_segment_path;

void init_app(void);
void deinit_app(void);

void second_display_layer_update_callback(Layer *layer, GContext* ctx);
void minute_display_layer_update_callback(Layer *layer, GContext* ctx);
void hour_display_layer_update_callback(Layer *layer, GContext* ctx);

void tick_every_second(struct tm *tick_time, TimeUnits units_changed);
void update_day(struct tm *tick_time);
void update_battery(void);

#endif