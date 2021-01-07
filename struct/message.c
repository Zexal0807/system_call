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

int readDelay(char *buffer, int *i){
	int delay = 0;
	//se trovo come delay -, allora segno 0
	if(*(buffer + *i) == '-')
		*(buffer + *i) = '0';
	
	while(*(buffer + *i)!=';'){
		delay = (delay * 10) + (int)(*(buffer + *i) - '0');
		*i = (*i) + 1;
	}
	return delay;
}

message* linetoStruct(
	char *buffer, 
	int *i
){

  process *s1 = createProcess('S', 1);
  process *s2 = createProcess('S', 2);
  process *s3 = createProcess('S', 3);

  process *r1 = createProcess('R', 1);
  process *r2 = createProcess('R', 2);
  process *r3 = createProcess('R', 3);


	int id, delay1, delay2, delay3;
	char *content;
	process *sender;
	process *receiver;
	char *communication;

	id = 0;
	while( *(buffer + *i) != ';'){
		id = (id * 10) + (int)(*(buffer + *i) - '0');
		*i = (*i) + 1;
	}

	//Mi muovo verso il prossimo dato
	*i = (*i) + 1;

	//conto la dimensione del messaggio
	int j = *i;
	int counter = 0;
	while(*(buffer + j) != ';'){
		j++;
		counter++;
	}

	//creo la stringa con il contenuto
	content = (char*) malloc(sizeof(char) * counter);
	for (j = 0; *(buffer + *i) != ';'; j++){
		*(content + j) = *(buffer + *i);
		*i = (*i) + 1;
	}

	//Mi muovo verso il prossimo dato
	//Il prossimo dato è il Sender, che riconosco dal
	//numero 
	*i = (*i) + 1;

	//mi assicuro che il carattere sia giusto
	if(*(buffer + *i) != 'S'){
		perror("Error in sender");
		exit(1);
	}
	*i = (*i) + 1;
	
	//In base al numero, trovo il sender corretto
	switch (*(buffer+*i)){
		case '1':
			sender = s1;
			break;
		case '2':
			sender = s2;
			break;
		case '3':
			sender = s3;
			break;
		default:
			perror("Error in sender");
			exit(1);
	}
	
	//Mi muovo verso il prossimo dato
	//Il prossimo dato è il Receiver, che riconosco 
	//sempre dal numero

	*i = (*i) + 2;

	if(*(buffer + *i) != 'R'){
		perror("Error in receiver");
		exit(1);
	}

	*i = (*i) + 1;

	//in base al numero, trovo il receiver corretto
	switch (*(buffer + *i)){
		case '1':
			receiver = r1;
			break;
		case '2':
			receiver = r2;
			break;
		case '3':
			receiver = r3;
			break;
		default:
			perror("Error in receiver");
			exit(1);
	}
	
	//mi avvio al prossimo dato
	*i = (*i) + 2;

	delay1 = 0;
	delay2 = 0;
	delay3 = 0;

	//se trovo come delay -, allora segno 0
	if(*(buffer + *i) == '-')
		*(buffer + *i) = '0';
	
	while(*(buffer + *i)!=';'){
		delay1 = (delay1 * 10) + (int)(*(buffer + *i) - '0');
		*i = (*i) + 1;
	}

	*i = (*i) + 1;
	
	//ripeto per il delay 2
	if(*(buffer + *i) == '-')
		*(buffer + *i) = '0';

	while(*(buffer+*i)!=';'){
		delay2=(delay2*10)+(int)(*(buffer+*i)-'0');
		*i=(*i)+1;
	}

	*i=(*i)+1;

	//ripeto per il delay 3
	if(*(buffer+*i)=='-')
		*(buffer+*i)='0';

	while(*(buffer+*i)!=';'){
		delay3 = (delay3 * 10) + (int)( *(buffer + *i) - '0');
		*i = (*i) + 1;
	}

	//avanzo al prossimo dato

	*i = (*i) + 1;

	//calcolo la dimensione della strinfa
	counter = 0;

	for(j = *i; *(buffer + j) != '\n'; j++)
		counter++;

	//creo la stringa
	communication = (char*)malloc(sizeof(char) * counter);


	for(j = 0; *(buffer+*i) != '\n' && *(buffer + *i) != '\0'; j++){
		*(communication + j) = *(buffer + *i);
		*i = (*i) + 1;
	}

	//creo il messaggio
	return createMessage(id, content, sender, receiver, delay1, delay2, delay3, communication);
}