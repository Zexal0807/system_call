/// @file defines.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche del progetto.

#include "defines.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void printLog(char *p, char *text){
	printf("%s\t:\t%s\n", p, text);
}

char *time_t2string(time_t time){
	char *s = (char*) malloc(sizeof(char) * 9);
	struct tm *info = localtime(&time);

	// Fix legal hour
	info->tm_hour = (info->tm_hour + 1) % 24;

	strftime(s, 9, "%H:%M:%S", info);
	return s;
}

void fileAhead(int *i){
  *i = (*i) + 1;
}

int readInt(char *buffer, int *i){
	int value = 0;
	// Finche non arrivo al ;
	while(*(buffer + *i) != ';'){
		// Moltiplico per 10 e aggiungo il nuovo valore convertito in int
		value = value * 10;
		value += atoi(buffer + *i);
		fileAhead(i);
	}
	return value;
}

void printChild(char *filename, child *data){
	int file;
	if(access(filename, F_OK) == 0){
		// File exist, open in append mode
		file = open(filename, O_WRONLY | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);

        ErrOpen(file);

	}else{
		// File not exist, create it, and print the header
		file = open(filename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
        
        ErrOpen(file);

		char headerBuffer[] = "Id;Pid\n";
		write(file, headerBuffer, strlen(headerBuffer));
	}

	// Print a line
	int chars = countChildChars(data);
	char *buffer  = (char*) malloc(sizeof(char) * chars);
	sprintf(buffer, "%s;%d\n", 
		process2string(data->process),
		data->pid
	);
	write(file, buffer, strlen(buffer));

	// Close file
	close(file);
}

void printTrafficInfo(char *filename, trafficInfo *data){
	int file;
	if(access(filename, F_OK) == 0){
		// File exist, open in append mode
		file = open(filename, O_WRONLY | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
        
        ErrOpen(file);
	
    }else{
		// File not exist, create it, and print the header
		file = open(filename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	
        ErrOpen(file);
    	
        char headerBuffer[] = "Id;Message;Id Sender;Id Receiver;Time arrival;Time departure\n";
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

void printHacklerAction(char *filename, hackletAction *data){
	int file;
	if(access(filename, F_OK) == 0){
		// File exist, open in append mode
		file = open(filename, O_WRONLY | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
        
        ErrOpen(file);

	}else{
		// File not exist, create it, and print the header
		file = open(filename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
		
        ErrOpen(file);

        char headerBuffer[] = "Id;Delay;Target;Action\n";
		write(file, headerBuffer, strlen(headerBuffer));
	}

	// Print a line
	int chars = countHacklerActionChars(data);
	char *buffer  = (char*) malloc(sizeof(char) * chars);
	sprintf(buffer, "%d;%d;%s;%s;\n", 
		data->id, 
		data->delay, 
		process2string(data->target),
		data->action
	);
	write(file, buffer, strlen(buffer));

	// Close file
	close(file);
}