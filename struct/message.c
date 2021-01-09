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

int dimMessage(char *buffer, int *i){
    int j; //var indice per il conto
	int counter = 0; //var per conto caratteri
    /*
    partendo dal punto in cui sono (indicato da i), avanzo l'indice j fino al prossimo ';' incrementando man mano il counter, che così finisce con il contenere il numero di caratteri
    */
	for(j=*i ; *(buffer + j) != ';'; j++)
		counter++;
    return counter;
}

message* linetoStruct(
	char *buffer, 
	int *i
){
    //variabili necessarie
	int id,     
		delay1,  
		delay2, 
		delay3,
        j;      //indice secondario
	char *content;  
	process *sender;  
	process *receiver; 
	char *communication;

	//Leggo l'id dal file trasformandolo in intero
	id = readInt(buffer, i);
	fileAhead(i);

	//ricavo la stringa con il contenuto
	content = (char*) malloc(sizeof(char) * dimMessage(buffer, i));
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
			ErrExit("Error in receiver\n");
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
	//creo la stringa
	communication = (char*)malloc(sizeof(char) * dimMessage(buffer, i));
	for(j = 0; *(buffer+*i) != '\n' && *(buffer + *i) != '\0'; j++){
		*(communication + j) = *(buffer + *i);
		fileAhead(i);
	}

	//creo il messaggio
	return createMessage(id, content, sender, receiver, delay1, delay2, delay3, communication);
}