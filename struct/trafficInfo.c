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