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

const int INCREASE_DELAY = 0;
const char* ACTION_INCREASE_DELAY = "IncreaseDealy";

const int REMOVE_MSG = 1;
const char* ACTION_REMOVE_MSG = "RemoveMsg";

const int SEND_MSG = 2;
const char* ACTION_SEND_MSG = "SendMsg";

const int SHUT_DOWN = 3;
const char* ACTION_SHUT_DOWN = "ShutDown";

hackletAction *createHackletAction(
	int id,
	int delay,
	process* target,
	int action
){

	hackletAction *a = (hackletAction*) malloc(sizeof(hackletAction));

	a->id = id;
	a->delay = delay;
	a->target = target;
	a->target = target;

	return a;
}

char *time_t2string(time_t time){
	char *s = (char*) malloc(sizeof(char) * 9);
	struct tm *info = localtime(&time);

// Fix legal hour

   info->tm_hour = (info->tm_hour+1)%24;

	strftime(s, 9, "%H:%M:%S", info);
	return s;
}

void printProcessList(char *filename, char type, process *p1, process *p2, process *p3){
	
	int file = open(filename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	char buffer[50];

	sprintf(buffer, "%s ID;PID\n%c1;%i\n%c2;%i\n%c3;%i\n", (type=='S' ? "SENDER" : "RECEIVER"), type, p1->pid, type, p2->pid, type, p3->pid);

	write(file, buffer, strlen(buffer));

	close(file);
}

int countTrafficChar(trafficInfo *t){
  int chars = 0;

  // Number of digit of the id
  chars += floor (log10 (abs (t->message->id))) + 1;
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
  chars +=5;
  // Add \n
  chars +=1;
  return chars;
}

void printTrafficInfo(char *filename, trafficInfo *data){
	int file;
	if(access(filename, F_OK) == 0){
		// File exist
		file = open(filename, O_WRONLY | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
	}else{
		// File not exist
		file = open(filename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
		char headerBuffer[] = "Id;Message;Id Sender;Id Receiver;Time arrival;Time departure\n";
		write(file, headerBuffer, strlen(headerBuffer));
	}

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

	close(file);
}