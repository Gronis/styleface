#include <pebble.h>
#include "renderer.h"
#include "const.h"

Window *window;
TextLayer *text_layer;
InverterLayer *inverter_layer;
Layer *second_display_layer;
Layer *minute_display_layer;
Layer *hour_display_layer;
bool invert_colors = false;

GPath *hour_divider_segment_path;
GPath *hour_segment_path;


static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // hour_layer
  hour_display_layer = layer_create(bounds);
  layer_set_update_proc(hour_display_layer, hour_display_layer_update_callback); 
  layer_add_child(window_layer, hour_display_layer);

  // minute_layer
  minute_display_layer = layer_create(bounds);
  layer_set_update_proc(minute_display_layer, minute_display_layer_update_callback); 
  layer_add_child(window_layer, minute_display_layer);

  // second_layer
  second_display_layer = layer_create(bounds);
  layer_set_update_proc(second_display_layer, second_display_layer_update_callback); 
  layer_add_child(window_layer, second_display_layer);

  // text_layer
  text_layer = text_layer_create((GRect) { .origin = { bounds.size.w / 2 - 30, bounds.size.h / 2 + 20}, .size = { 60, 20 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_background_color(text_layer, GColorBlack);
  text_layer_set_text_color(text_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));

  // Init the hour segment path
  hour_divider_segment_path = gpath_create(&HOUR_DIVIDER_SEGMENT_PATH_POINTS);
  gpath_move_to(hour_divider_segment_path, grect_center_point(&bounds));

  hour_segment_path = gpath_create(&HOUR_SEGMENT_PATH_POINTS);
  gpath_move_to(hour_segment_path, grect_center_point(&bounds));

  //invert layer
  inverter_layer = inverter_layer_create(bounds);
  if(invert_colors){
    layer_add_child(window_layer, (Layer*) inverter_layer);
  }
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
  inverter_layer_destroy(inverter_layer);
  layer_destroy(second_display_layer);
  layer_destroy(minute_display_layer);
  layer_destroy(hour_display_layer);
}

static void init(void) {
  window = window_create();
  window_set_background_color(window, GColorBlack);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  //add callback every seconed
  tick_timer_service_subscribe(SECOND_UNIT, tick_every_second);

  const bool animated = true;
  window_stack_push(window, animated);

  init_app();
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
