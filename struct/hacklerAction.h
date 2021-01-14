#include <stdlib.h>
#include <string.h>

#include "message.h"

/*
	Struttura di un'azione
	 - id : id della azione
	 - delay : Numero del processo
	 - target : PID del processo
	 - action : Azione del disturbatore
*/
typedef struct {
	int id;
	int delay;
	process* target;
	char* action;
} hacklerAction;

/*
	Funzione per creare un hacklet Action
*/
hacklerAction *createHacklerAction(int id, int delay, process* target, char* action);

char *openHackler(char *pathname);
/*
	Funzione per creare un messaggio partendo da una stringa
*/
hacklerAction* line2hacklerAction(char *buffer);

/*
	Funzione per stampare un hacklerAction su file
*/
void printHacklerAction(char *filename, hacklerAction *data);
