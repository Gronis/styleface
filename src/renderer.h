#include <pebble.h>

#ifndef renderer_h
#define renderer_h

extern GPath *hour_divider_segment_path;
extern GPath *hour_segment_path;

void second_display_layer_update_callback(Layer *layer, GContext* ctx);
void minute_display_layer_update_callback(Layer *layer, GContext* ctx);
void hour_display_layer_update_callback(Layer *layer, GContext* ctx);

#endif