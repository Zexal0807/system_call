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
	int delayS1;
	int delayS2;
	int delayS3;
    int delayR1;
	int delayR2;
	int delayR3;
	char* comunication;
} message;

/*
	Funzione per create un messaggio
*/
message *createMessage(int id, char* content, process* sender, process* receiver, int delay1, int delay2, int delay3, char* comunication);


/*
	Funzione per creare un messaggio partendo da una riga
*/
message* line2message(char *buffer);

/*
	Funzione per creare un messaggio in csv partendo da una struct messaggio
*/
char* message2line(message *m);
