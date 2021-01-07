/// @file defines.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche del progetto.

#include "defines.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>

void printLog(char *p, char *text){
	printf("%s\t:%s\n", p, text);
}

char *time_t2string(time_t time){
	char *s = (char*) malloc(sizeof(char) * 9);
	struct tm *info = localtime(&time);

	// Fix legal hour
	info->tm_hour = (info->tm_hour + 1) % 24;

	strftime(s, 9, "%H:%M:%S", info);
	return s;
}

void printChildList(
  char *filename, 
  char type, 
  child *p1, 
  child *p2, 
  child *p3
){
	
	int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU | S_IRWXG | S_IRWXO);
	char buffer[50];

	sprintf(buffer, "%s ID;PID\n%c1;%i\n%c2;%i\n%c3;%i\n", (type=='S' ? "SENDER" : "RECEIVER"), type, p1->pid, type, p2->pid, type, p3->pid);

	write(file, buffer, strlen(buffer));

	close(file);
}

int countTrafficChar(trafficInfo *t){
	int chars = 0;

	// Number of digit of the id
	chars += floor(log10(t->message->id)) + 1;
	// Length of the content
	chars += strlen(t->message->content);
	// Sender process is S#
	chars += 2;
	// Receiver process is R#
	chars += 2;
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
	}else{
		// File not exist, create it, and print the header
		file = open(filename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
		char headerBuffer[] = "Id;Message;Id Sender;Id Receiver;Time arrival;Time departure\n";
		write(file, headerBuffer, strlen(headerBuffer));
	}

  // Print a line
	int chars = countTrafficChar(data);
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