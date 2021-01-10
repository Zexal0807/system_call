#include <stdlib.h>
#include <string.h>

#include "process.h"

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
} hackletAction;

/*
	Funzione per creare un hacklet Action
*/
hackletAction *createHackletAction(int id, int delay, process* target, char* action);

int countHacklerActionChars(hackletAction *h);

/*
	Funzione per creare un messaggio partendo da un buffer usando l'indice da cui inizia la riga
*/
hackletAction* line2haclerAction(char *buffer, int *i);
