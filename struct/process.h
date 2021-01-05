#pragma once

/*
	Struttura di un processo
	 - type : Tipo del processo [S: Sender, R: Reciver]
	 - number : Numero del processo
	 - pid : PID del processo
	Target Z0 : ALL
*/
typedef struct {
	char type;
	int number;
	int pid;
} process;

process *createProcess(char type, int number, int pid);

process *createProcessFromString(char *str);

char *process2string(process *p);
