#pragma once

#include "process.h"

/*
	Struttura di un processo
	 - process : Processo 
	 - pid : PID del processo
*/
typedef struct {
	process *process;
	int pid;
} child;

child *createChild(process * p, int pid);

/*
	Funzione per stampare un child su file
*/
void printChild(char * filename, child * data);
