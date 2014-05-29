#include "renderer.h"
#include "stdio.h"
#include "const.h"

static char *day_string;
static char *battery_string;

/** 
	Draws a circle that exists of dots
	
	circle_radius: Radius of the whole circle
	max_num_of_dots: how many dots there should be in the whole circle
	dot_radius: how big each individual dot should be
	progress: a value from 0 to max_progress. Determines how many dots to draw. 
	max_progress: When equals to progress, all dots are rendered
 */
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
  		//scale the last dot if needed
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
	render_circle_with_dots(layer, ctx, circle_radius, max_num_of_dots, dot_radius, t->tm_sec == 0? 60 : t->tm_sec, 60);
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

	for(int i = t->tm_hour % 12 == 0? 12 : t->tm_hour % 12; i < 12; i++){
  		gpath_rotate_to(hour_segment_path, (TRIG_MAX_ANGLE / 360) * i * 30);
  		gpath_draw_filled(ctx, hour_segment_path);
  	}


  	graphics_fill_circle(ctx, center, 41);

}

void tick_every_second(struct tm *tick_time, TimeUnits units_changed){
	layer_mark_dirty(second_display_layer);
	update_day(tick_time);
	update_battery();
}

void update_day(struct tm *tick_time){
	strcpy(day_string, WEEKDAYS[tick_time->tm_wday]);
	strcat(day_string, itoa(tick_time->tm_mday));
	text_layer_set_text(day_text_layer, day_string);
}

void update_battery(void){
	int battery = battery_state_service_peek().charge_percent;
	strcpy(battery_string, itoa(battery));
	strcat(battery_string, "%");
	text_layer_set_text(battery_text_layer, battery_string);
}

void init_app(void){
	day_string = malloc(sizeof(char) * 8);
	battery_string = malloc(sizeof(char) * 8);

	//Call time at the beginning
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
 
	//Manually call the tick handler to refresh GUI
	tick_every_second(t, SECOND_UNIT);
}

void deinit_app(void){
	free(day_string);
	free(battery_string);
}