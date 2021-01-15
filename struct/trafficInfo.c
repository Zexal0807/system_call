#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#include "trafficInfo.h"
#include "../err_exit.h"

#ifndef TRAFFINC_INFO_FILE_HEADER
#define TRAFFINC_INFO_FILE_HEADER "Id;Message;Id Sender;Id Receiver;Time arrival;Time departure\n"
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

char *time_t2string(time_t time){
	char *s = (char*) malloc(sizeof(char) * 9);
	struct tm *info = localtime(&time);

	// Fix legal hour
	info->tm_hour = (info->tm_hour + 1) % 24;

	strftime(s, 9, "%H:%M:%S", info);
	return s;
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
	sprintf(buffer, "%d;%s;%s;%s;%s;%s\n", 
		data->message->id, 
		data->message->content, 
		process2string(data->message->sender),
		process2string(data->message->receiver),
		time_t2string(data->arrival),
		time_t2string(data->departure)
	);
	write(file, buffer, strlen(buffer));

	// Close file
	close(file);
}