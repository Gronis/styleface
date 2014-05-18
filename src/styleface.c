#include <pebble.h>
#include "app.h"

Window *window;
TextLayer *text_layer;
InverterLayer *inverter_layer;
bool invert_colors = false;

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 100 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_background_color(text_layer, GColorBlack);
  text_layer_set_text_color(text_layer, GColorClear);

  inverter_layer = inverter_layer_create(GRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

  // add layers to window layer
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
  //invert all colors
  if(invert_colors){
    layer_add_child(window_layer, (Layer*) inverter_layer);
  }
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
  inverter_layer_destroy(inverter_layer);
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
