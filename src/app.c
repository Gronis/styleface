#include <string.h>
#include "app.h"
#include "stdio.h"


static int time_num;
static char *string;

void init_app(void){
	time_num = 0;
	string = malloc(sizeof(char) * 20);
}

void tick_every_second(struct tm *tick_time, TimeUnits units_changed){
	print_time(tick_time->tm_hour, tick_time->tm_min, tick_time->tm_sec);
}

void print_time(int hour, int min, int sec){
	strcpy(string, hour >= 10 ? "" : "0");
	strcat(string, itoa(hour));
	strcat(string, min >= 10 ? ":" : ":0");
	strcat(string, itoa(min));
	strcat(string, sec >= 10 ? ":" : ":0");
	strcat(string, itoa(sec));
	text_layer_set_text(text_layer, string);
}

