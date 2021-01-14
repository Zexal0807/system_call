#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "message.h"
#include "../err_exit.h"

message *createMessage(int id,
	char* content,
	process* sender,
	process* receiver,
	int delay1,
	int delay2,
	int delay3,
	char* comunication
) {

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

/*
	partendo dal punto in cui sono (indicato da i), avanzo l'indice j fino al prossimo ';' incrementando man mano il counter, che così finisce con il contenere il numero di caratteri
	*/
int dimMessage(char *buffer, int *i){
	int j; //var indice per il conto
	int counter = 0; //var per conto caratteri

	for(j=*i ; *(buffer + j) != ';'; j++)
		counter++;
	return counter;
}

void fileAhead(int *i){
	*i = (*i) + 1;
}

int dimComunication(char *buffer, int *i){
	int j; //var indice per il conto
	int counter = 0; //var per conto caratteri

	for(j=*i ; *(buffer + j) != '\n' && *(buffer + j) != 0x0; j++)
		counter++;
	return counter;
}

int readInt(char *buffer, int *i){
	int value = atoi(buffer + *i);
	int tmp = log10(value) + 1;
	for(int j = 0; j < tmp; j++){
		fileAhead(i);
	}
	return value;
}

message* line2message(
	char *buffer, 
	int *i
){
	
	char *communication;

	//Leggo l'id dal file trasformandolo in intero
	int id = readInt(buffer, i);
	fileAhead(i);

	//ricavo la stringa con il contenuto
	char *content = (char*) malloc(sizeof(char) * dimMessage(buffer, i));
	int j;
	for (j = 0; *(buffer + *i) != ';'; j++){
		*(content + j) = *(buffer + *i);
		fileAhead(i);
	}
	fileAhead(i);

	//Analizzo il Sender
	//mi assicuro che il un Sender
	if(*(buffer + *i) != 'S'){
		ErrExit("Error in sender\n");
	}
	fileAhead(i);

	//In base al numero, trovo il sender corretto
	process *sender;  
	switch (*(buffer+*i)){
		case '1':
			sender = SENDER_1();
			break;
		case '2':
			sender = SENDER_2();
			break;
		case '3':
			sender = SENDER_3();
			break;
		default:
			ErrExit("Error in sender\n");
	}
	fileAhead(i);
	fileAhead(i);

	//Analizzo il Receiver
	//mi assicuro che il un Receiver
	if(*(buffer + *i) != 'R'){
		ErrExit("Error in receiver\n");
	}
	fileAhead(i);

	//in base al numero, trovo il Receiver corretto
	process *receiver;
	switch (*(buffer + *i)){
		case '1':
			receiver = RECEIVER_1();
			break;
		case '2':
			receiver = RECEIVER_2();
			break;
		case '3':
			receiver = RECEIVER_3();
			break;
		default:
			ErrExit("Error in receiver\n");
	}
	fileAhead(i);
	fileAhead(i);

	//Per la lettura dei delay devo controllare se sono -, nel caso li trasfomo in 0
	if(*(buffer + *i) == '-')
		*(buffer + *i) = '0';
	int delay1 = readInt(buffer, i);
	fileAhead(i);

	if(*(buffer + *i) == '-')
		*(buffer + *i) = '0';
	int delay2 = readInt(buffer, i);
	fileAhead(i);

	if(*(buffer + *i) == '-')
		*(buffer + *i) = '0';
	int delay3 = readInt(buffer, i);
	fileAhead(i);

	//Analizzo il tipo di comunicazion
	//creo la stringa
	communication = (char*)malloc(sizeof(char) * dimComunication(buffer, i));
	for(j = 0; *(buffer+*i) != '\n' && *(buffer + *i) != '\0'; j++){
		*(communication + j) = *(buffer + *i);
		fileAhead(i);
	}

	//creo il messaggio
	return createMessage(id, content, sender, receiver, delay1, delay2, delay3, communication);
}