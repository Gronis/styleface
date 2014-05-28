#include <string.h>
#include "app.h"
#include "stdio.h"

static char *string;

void init_app(void){
	string = malloc(sizeof(char) * 20);

	//Call time at the beginning
	struct tm *t;
	time_t temp;
	temp = time(NULL);
	t = localtime(&temp);
 
	//Manually call the tick handler to refresh GUI
	tick_every_second(t, SECOND_UNIT);
}

void tick_every_second(struct tm *tick_time, TimeUnits units_changed){
	layer_mark_dirty(second_display_layer);
	layer_mark_dirty(minute_display_layer);
	layer_mark_dirty(hour_display_layer);
	//print_time(tick_time->tm_hour, tick_time->tm_min, tick_time->tm_sec);
}

void print_time(int hour, int min, int sec){

	strcpy(string, hour >= 10 ? "" : "0");
	strcat(string, itoa(hour));
	strcat(string, min >= 10 ? ":" : ":0");
	strcat(string, itoa(min));
	strcat(string, sec >= 10 ? ":" : ":0");
	strcat(string, itoa(sec));
	text_layer_set_text(text_layer, string);

	//strftime(string, sizeof("00:00"), "%H:%M", tick_time);
}

