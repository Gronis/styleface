#include "renderer.h"



void second_display_layer_update_callback(Layer *layer, GContext* ctx){
	APP_LOG(APP_LOG_LEVEL_DEBUG, "rendering second layer");

	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	int32_t radius = 70;

	GRect bounds = layer_get_bounds(layer);
  	GPoint center = grect_center_point(&bounds);
  	GPoint point;

  	graphics_context_set_fill_color(ctx, GColorWhite);

  	for(int i = 0; i <= t->tm_sec; i++){
  		int32_t angle = TRIG_MAX_ANGLE * i / 60 + TRIG_MAX_ANGLE / 120;
  		int32_t x = sin_lookup(angle) * radius / TRIG_MAX_RATIO;
  		int32_t y = -cos_lookup(angle) * radius / TRIG_MAX_RATIO;
  		point.x = center.x + x;
  		point.y = center.y + y;

  		graphics_fill_circle(ctx, point, 2);
  	}

}

void minute_display_layer_update_callback(Layer *layer, GContext* ctx){
	APP_LOG(APP_LOG_LEVEL_DEBUG, "rendering minute layer");

	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	int32_t radius = 60;

	GRect bounds = layer_get_bounds(layer);
  	GPoint center = grect_center_point(&bounds);
  	GPoint point;

  	graphics_context_set_fill_color(ctx, GColorWhite);

  	for(int i = 0; i < 36 * t->tm_min / 60 ; i++){
  		int32_t angle = TRIG_MAX_ANGLE * i / 36 + TRIG_MAX_ANGLE / 72;
  		int32_t x = sin_lookup(angle) * radius / TRIG_MAX_RATIO;
  		int32_t y = -cos_lookup(angle) * radius / TRIG_MAX_RATIO;
  		point.x = center.x + x;
  		point.y = center.y + y;

  		graphics_fill_circle(ctx, point, 4);
  	}

}

void hour_display_layer_update_callback(Layer *layer, GContext* ctx){
	APP_LOG(APP_LOG_LEVEL_DEBUG, "rendering hour layer");
	
	time_t now = time(NULL);
	struct tm *t = localtime(&now);

  	GRect bounds = layer_get_bounds(layer);
  	GPoint center = grect_center_point(&bounds);

  	graphics_context_set_fill_color(ctx, GColorWhite);

  	graphics_fill_circle(ctx, center, 50);

  	graphics_context_set_fill_color(ctx, GColorBlack);

  	// Note: I had intended to use the `GCompOpAssignInverted` mode here
  	//       but it appears it's ignored for path/shape drawing.

  	for(int i = 0; i < 12; i++){
  		gpath_rotate_to(hour_divider_segment_path, (TRIG_MAX_ANGLE / 360) * i * 30);
  		gpath_draw_filled(ctx, hour_divider_segment_path);
  	}

	for(int i = t->tm_hour % 12; i < 12; i++){
  		gpath_rotate_to(hour_segment_path, (TRIG_MAX_ANGLE / 360) * i * 30);
  		gpath_draw_filled(ctx, hour_segment_path);
  	}


  	graphics_fill_circle(ctx, center, 41);

}