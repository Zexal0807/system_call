/// @file defines.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche del progetto.

#include "defines.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>

void printLog(char *p, char *text){
    printf("%s\t:%s\n", p, text);
}

process *createProcess(
	char type,
	int number,
	int pid){

    process *p = (process*) malloc(sizeof(process));

    p->type = type;
    p->number = number;
    p->pid = pid;
    
    return p;
}

message *createMessage(
	int id, 
	char* content, 
	process* sender, 
	process* receiver, 
	int delay1, 
	int delay2, 
	int delay3, 
  char* comunication
){

    message *m = (message*) malloc(sizeof(message));

    m->id = id;
    m->content = strdup(content);
    m->sender = sender;
    m->receiver = receiver;
    m->delay1 = delay1;
    m->delay2 = delay2;
    m->delay3 = delay3;
    m->comunication = comunication;

    return m;
}

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

char *process2string(process *p){
    char *s = (char*) malloc(sizeof(char) * 2);
    sprintf(s, "%c%d", p->type, p->number);
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
}