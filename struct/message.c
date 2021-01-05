#pragma once

#include "process.c"

/*
	Struttura del messaggio
	 - id : Identificativo del messaggio
	 - content : Stringa del messsaggio
	 - sender : Processo che deve spedire il messaggio
	 - reciver : Pocesso che deve riceve il messsaggio
	 - delay1 : Tempo di attessa in S1 e R1
	 - delay2 : Tempo di attessa in S2 e R2
	 - delay3 : Tempo di attessa in S3 e R3
	 - comunication : Tipo di comunciazione
*/
typedef struct {
	int id;
	char *content;
	process *sender;
	process *receiver;
	int delay1;
	int delay2;
	int delay3;
	char* comunication;
} message;


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