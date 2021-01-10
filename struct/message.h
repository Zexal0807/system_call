#pragma once

#include "process.h"
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

/*
	Funzione per create un messaggio
*/
message *createMessage(int id, char* content, process* sender, process* receiver, int delay1, int delay2, int delay3, char* comunication);


void fileAhead(int *i);

int readInt(char *buffer, int *i);
/*
	Funzione per creare un messaggio partendo da un buffer usando l'indice da cui inizia la riga
*/
message* line2message(char *buffer, int *i);
