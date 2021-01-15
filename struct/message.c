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
	
    int id;
    char *content;
    process *sender;
    process *receiver; 
    int delay1, delay2, delay3;
    char *communication;
    
    char *endline;
    char *field=strtok_r(buffer, ";", &endline);
	
    counter=0;

    while(field!=NULL){
        switch (counter){
            case 0:
                id=atoi(field);
                break;
            case 1:
                content=strdup(field);
                break;
            case 2:
                sender=string2process(field);
                break;
            case 3:
                receiver=string2process(field);
                break;
            case 4:
                delay1=atoi(field);
                break;
            case 5:
                delay2=atoi(field);
                break;
            case 6:
                delay3=atoi(field);
                break;
            case 7:
                communication=strdup(field);
                break;
            default
                break;

        }
        
        field = strtok_r(NULL, ";", &end_buffer);
        counter++;
    }

	//creo il messaggio
	return createMessage(id, content, sender, receiver, delay1, delay2, delay3, communication);
}