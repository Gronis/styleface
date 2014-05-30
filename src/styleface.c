#include <pebble.h>
#include "renderer.h"
#include "const.h"

Window *window;
TextLayer *day_text_layer;
TextLayer *battery_text_layer;
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

  // day_text_layer
  day_text_layer = text_layer_create((GRect) { .origin = { bounds.size.w / 2 - 20, bounds.size.h / 2 + 20}, .size = { 40, 18 } });
  text_layer_set_font(day_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(day_text_layer, GTextAlignmentCenter);
  text_layer_set_background_color(day_text_layer, GColorBlack);
  text_layer_set_text_color(day_text_layer, GColorClear);
  layer_add_child(window_layer, text_layer_get_layer(day_text_layer));

  // battery_text_layer
  battery_text_layer = text_layer_create((GRect) { .origin = { bounds.size.w - 30, 0}, .size = { 30, 18 } });
  text_layer_set_font(battery_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(battery_text_layer, GTextAlignmentCenter);
  text_layer_set_background_color(battery_text_layer, GColorClear);
  text_layer_set_text_color(battery_text_layer, GColorWhite);
  layer_add_child(window_layer, text_layer_get_layer(battery_text_layer));

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
  text_layer_destroy(day_text_layer);
  inverter_layer_destroy(inverter_layer);
  layer_destroy(second_display_layer);
  layer_destroy(minute_display_layer);
  layer_destroy(hour_display_layer);
  gpath_destroy(hour_divider_segment_path);
  gpath_destroy(hour_segment_path);
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
  deinit_app();
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
