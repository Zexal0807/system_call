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

child *createChild(char type, int number, int pid);
