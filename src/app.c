#include <string.h>
#include "app.h"
#include "stdio.h"


static int time_num;
static char *string;

void init_app(void){
	time_num = 2000;
	string = malloc(sizeof(char) * 20);
}

void tick_every_second(struct tm *tick_time, TimeUnits units_changed){
	strcpy(string, "Hej ");
	strcat(string, itoa(time_num++));
	//sprintf(string, "%d", time_num++);
	text_layer_set_text(text_layer, string);
}

