#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "hacklerAction.h"
#include "../err_exit.h"

#ifndef HACKLER_ACTION_FILE_HEADER
#define HACKLER_ACTION_FILE_HEADER "Id;Delay;Target;Action\n"
#endif

hacklerAction * createHacklerAction(
	int id,
	int delay,
	process * target,
	char * action
){

	hacklerAction * a = (hacklerAction *) malloc(sizeof(hacklerAction));

	a->id = id;
	a->delay = delay;
	a->target = target;
	a->action = action;

	return a;
}

int countHacklerActionChars(hacklerAction * h){
	int chars = 0;

	// Number of digit of the id
	chars += floor(log10(h->id)) + 1;
	// Number of digit of the delay
	chars += floor(log10(h->delay)) + 1;
	// Target process
	chars += strlen(process2string(h->target));
	// Target process
	chars += strlen(h->action);
	// Add the ;
	chars += 3;
	// Add \n
	chars += 1;
	return chars;
}

char * openHackler(char * pathname){
	int file, dim, readed;
	char * buffer;

	file = open(pathname, O_RDONLY);

	ErrOpen(file);

	dim = lseek(file, 0L, SEEK_END);
	lseek(file, 0L, SEEK_SET);

	buffer = (char *)malloc(dim * sizeof(char));

	readed = read(file, buffer, dim);

	if(readed == -1)
		ErrExit("File non leggibile\n");

	close(file);

	return buffer;
}

hacklerAction * line2hacklerAction(
	char * buffer
){
	char * end_buffer;

	int id = 0;
	int delay = 0;
	process * target = SENDER_1();
	char * action = "ShutDown";

	int counter = 0;

	char * field = strtok_r(buffer, ";", &end_buffer);
	while(field != NULL){
		switch(counter){
			case 0:
				id = atoi(field);
				break;
			case 1:
				delay = atoi(field);
				break;
			case 2:
				target = string2process(field);
				break;
			case 3:
				action = strdup(field);
				break;
			default:
				//Ignored
				break;
		}
		field = strtok_r(NULL, ";", &end_buffer);
		counter++;
	}
	return createHacklerAction(id, delay, target, action);
}

void printHacklerActionHeader(char * filename){
	int file = open(filename, O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	
	if(file == -1)
		ErrOpen(file);
	
	char headerBuffer[] = HACKLER_ACTION_FILE_HEADER;
	write(file, headerBuffer, strlen(headerBuffer));

	// Close file
	close(file);
}

void printHacklerAction(char * filename, hacklerAction * data){
	int file;
	if(access(filename, F_OK) == 0){
		// File exist, open in append mode
		file = open(filename, O_WRONLY | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
		ErrOpen(file);
	}else{
		// File not exist, create it, and print the header
		printHacklerActionHeader(filename)
	}

	// Print a line
	int chars = countHacklerActionChars(data);
	char * buffer  = (char*) malloc(chars * sizeof(char));
	sprintf(buffer, "%d;%d;%s;%s\n", 
		data->id, 
		data->delay, 
		process2string(data->target),
		data->action
	);
	write(file, buffer, strlen(buffer));

	// Close file
	close(file);
}