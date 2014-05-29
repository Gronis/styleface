#include "renderer.h"

// draws a circle that exists of dots

//progress, a value from 0 to 59 depending how many dots to draw
void render_circle_with_dots(Layer *layer, GContext* ctx, int circle_radius, int max_num_of_dots, int dot_radius, int progress, int max_progress){

	GRect bounds = layer_get_bounds(layer);
  	GPoint center = grect_center_point(&bounds);
  	GPoint point;

  	graphics_context_set_fill_color(ctx, GColorWhite);
  	int num_of_dots = max_num_of_dots * progress / max_progress;
  	
  	for(int i = 0; i <= num_of_dots; i++){
  		int32_t angle = TRIG_MAX_ANGLE * i / max_num_of_dots + TRIG_MAX_ANGLE / (max_num_of_dots * 2);
  		int32_t x = sin_lookup(angle) * circle_radius / TRIG_MAX_RATIO;
  		int32_t y = -cos_lookup(angle) * circle_radius / TRIG_MAX_RATIO;
  		point.x = center.x + x;
  		point.y = center.y + y;
  		//last dot
  		int scale = max_num_of_dots * progress - i * max_progress; 
  		int size = scale > max_progress? dot_radius: scale < 0? 0: dot_radius * scale / max_progress;
  		if(size > 0) {
  			graphics_fill_circle(ctx, point, size);
  		}
  	}
}

void second_display_layer_update_callback(Layer *layer, GContext* ctx){
	APP_LOG(APP_LOG_LEVEL_DEBUG, "rendering second layer");

	time_t now = time(NULL);
	struct tm *t = localtime(&now);

	int circle_radius = 70;
	int max_num_of_dots = 60;
	int dot_radius = 2;
	render_circle_with_dots(layer, ctx, circle_radius, max_num_of_dots, dot_radius, t->tm_sec, 60);
}

void minute_display_layer_update_callback(Layer *layer, GContext* ctx){
	APP_LOG(APP_LOG_LEVEL_DEBUG, "rendering minute layer");

	time_t now = time(NULL);
	struct tm *t = localtime(&now);

	int circle_radius = 60;
	int max_num_of_dots = 36;
	int dot_radius = 4;
	render_circle_with_dots(layer, ctx, circle_radius, max_num_of_dots, dot_radius, t->tm_min, 60);
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