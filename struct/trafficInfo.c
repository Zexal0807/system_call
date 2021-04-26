#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "trafficInfo.h"
#include "../err_exit.h"
#include "../defines.h"

#ifndef TRAFFINC_INFO_FILE_HEADER
#define TRAFFINC_INFO_FILE_HEADER "ID;Message;IDSender;IDReceiver;TimeArrival;TimeDeparture\n"
#endif

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
	chars += 1;
	// Receiver process is R#
	chars += 1;
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

void printTrafficInfo(char *filename, trafficInfo *data){
	int file;
	if(access(filename, F_OK) == 0){
		// File exist, open in append mode
		file = open(filename, O_WRONLY | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
		ErrOpen(file);
}	else{
		// File not exist, create it, and print the header
		file = open(filename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
		ErrOpen(file);
		char headerBuffer[] = TRAFFINC_INFO_FILE_HEADER;
		write(file, headerBuffer, strlen(headerBuffer));
	}

	// Print a line
	int chars = countTrafficInfoChars(data);
	char *buffer  = (char*) malloc(sizeof(char) * chars);
	sprintf(buffer, "%d;%s;%d;%d;%s;%s\n", 
		data->message->id, 
		data->message->content, 
		data->message->sender->number,
		data->message->receiver->number,
		time_t2string(data->arrival),
		time_t2string(data->departure)
	);
	write(file, buffer, strlen(buffer));

	// Close file
	close(file);
}