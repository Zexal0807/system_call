#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "message.h"
#include "../defines.h"
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

message* line2message(
	char *buffer
){
	int id;
	char *content;
	process *sender;
	process *receiver; 
	int delay1, 
		delay2,
		delay3;
	char *comunication;
	
	char *field = strtok(buffer, ";");
	
	int counter = 0;

	while(field != NULL){
		switch (counter){
			case 0:
				id = atoi(field);
				break;
			case 1:
				content = strdup(field);
				break;
			case 2:
				sender = string2process(field);
				break;
			case 3:
				receiver = string2process(field);
				break;
			case 4:
				delay1 = atoi(field);
				break;
			case 5:
				delay2 = atoi(field);
				break;
			case 6:
				delay3 = atoi(field);
				break;
			case 7:
				comunication = strdup(field);
				break;
			default:
				break;
		}
		
		field = strtok(NULL, ";");
		counter++;
	}

	//creo il messaggio
	return createMessage(id, content, sender, receiver, delay1, delay2, delay3, comunication);
}

char* message2line(message *m){
    char * message = (char*) malloc(sizeof(char) * MAX_MESSAGE_LENGTH);
    sprintf(
        message, 
        "%d;%s;%s;%s;%d;%d;%d;%s",
        m->id,
        m->content,
        process2string(m->sender),
        process2string(m->receiver),
        m->delay1,
        m->delay2,
        m->delay3,
        m->comunication
    );
    return message;
}