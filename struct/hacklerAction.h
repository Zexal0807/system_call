#include <stdlib.h>
#include <string.h>

#include "process.h"

/*
	Struttura di un'azione
	 - id : id della azione
	 - delay : Numero del processo
	 - target : PID del processo
	 - action : PID del processo
*/
typedef struct {
	int id;
	int delay;
	process* target;
	char* action;
} hackletAction;

/*
	Funzione per creare un hacklet Action
*/
hackletAction *createHackletAction(int id, int delay, process* target, char* action);