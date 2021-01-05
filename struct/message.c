#include <stdlib.h>
#include <string.h>
#include "message.h"

message *createMessage(int id,
	char* content,
	process* sender,
	process* receiver,
	int delay1,
	int delay2,
	int delay3,
	char* comunication) {

	message *m=(message*) malloc(sizeof(message));

	m->id=id;
	m->content=strdup(content);
	m->sender=sender;
	m->receiver=receiver;
	m->delay1=delay1;
	m->delay2=delay2;
	m->delay3=delay3;
	m->comunication=comunication;

	return m;
}