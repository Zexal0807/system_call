/// @file defines.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche del progetto.

#include "defines.h"

const process SENDER_1 = {'S', 1};
const process SENDER_2 = {'S', 2};
const process SENDER_3 = {'S', 3};

const process RECIVER_1 = {'R', 1};
const process RECIVER_2 = {'R', 2};
const process RECIVER_2 = {'R', 3};

const process ALL_PROCESS = {'Z', 0};

const int MESSAGE_QUEUE = 1;
const int SHARED_MEMORY = 2;
const int FIFO = 3;

message *createMessage(
	int id, 
	char* content, 
	process sender, 
	process reciver, 
	int delay1, 
	int delay2, 
	int delay3, 
    int comunication
){

    message *m = (message*) malloc(sizeof(message));

    m->id = id;
    m->content = strdup(content);
    m->sender = sender;
    m->reciver = reciver;
    m->delay1 = delay1;
    m->delay2 = delay2;
    m->delay3 = delay3;
    m->comunication = comunication;

    return m;
}

hackletAction *createHackletAction(
	int id,
	int delay,
	process target,
	int action
){

    hackletAction *a = (hackletAction*) malloc(sizeof(hackletAction));

    a->id = id;
    a->delay = delay;
    a->target = target;
    a->target = target;

    return a;
}


char *t_time2string(t_time time){
    char *s = (char*) malloc(sizeof(char) * 8);
    strftime(s, 8, "%H:%M:%S", time);
    return s;
}