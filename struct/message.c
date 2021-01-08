#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "message.h"

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

int dimMessage(int *i){
    int j;
	int counter = 0;
    //Conto i caratteri fino al prossimo ;
	for(j=*i ; *(buffer + j) != ';'; j++)
		counter++;
}

message* linetoStruct(
	char *buffer, 
	int *i
){

	int id, 
		delay1,
		delay2, 
		delay3,
        dim;
	char *content;
	process *sender;
	process *receiver;
	char *communication;

	//Leggo l'id come intero
	id = readInt(buffer, i);
	fileAhead(i);

	//Conto la dimensione del messaggio
	dim=dimMessage(i);

	//creo la stringa con il contenuto
	content = (char*) malloc(sizeof(char) * dim);
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
			perror("Error in receiver\n");
			exit(1);
	}
	fileAhead(i);
	fileAhead(i);

	//Per la lettura dei delay devo controllare se sono -, nel caso li trasfomo in 0
	if(*(buffer + *i) == '-')
		*(buffer + *i) = '0';
	delay1 = readInt(buffer, i);
	fileAhead(i);

	if(*(buffer + *i) == '-')
		*(buffer + *i) = '0';
	delay2 = readInt(buffer, i);
	fileAhead(i);

	if(*(buffer + *i) == '-')
		*(buffer + *i) = '0';
	delay3 = readInt(buffer, i);
	fileAhead(i);

	//Analizzo il tipo di comunicazion
	//calcolo la dimensione della stringa
	dim=dimMessage(i);

	//creo la stringa
	communication = (char*)malloc(sizeof(char) * dim);
	for(j = 0; *(buffer+*i) != '\n' && *(buffer + *i) != '\0'; j++){
		*(communication + j) = *(buffer + *i);
		*i = (*i) + 1;
	}

	//creo il messaggio
	return createMessage(id, content, sender, receiver, delay1, delay2, delay3, communication);
}