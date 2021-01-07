#include <stdlib.h>

#include "trafficInfo.h"

trafficInfo *createTrafficInfo(
	message* message,
	time_t arrival,
	time_t departure
){

	trafficInfo *t = (trafficInfo*) malloc(sizeof(trafficInfo));

	t->message = message;
	t->arrival = arrival;
	t->departure = departure;

	return t;
}

int countTrafficInfoChars(trafficInfo *t){
	int chars = 0;

	// Number of digit of the id
	chars += floor(log10(t->message->id)) + 1;
	// Length of the content
	chars += strlen(t->message->content);
	// Sender process is S#
	chars += strlen(process2string(t->message->sender));
	// Receiver process is R#
	chars += strlen(process2string(t->message->receiver));
	// time arrival is HH:MM:SS
	chars += 8;
	// time departure is HH:MM:SS
	chars += 8 ;
	// Add the ;
	chars += 5;
	// Add \n
	chars += 1;
	return chars;
}
