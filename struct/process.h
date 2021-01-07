#pragma once

/*
	Struttura di un processo
	 - type : Tipo del processo [S: Sender, R: Reciver]
	 - number : Numero del processo
	Target Z0 : ALL
*/
typedef struct {
	char type;
	int number;

} process;

process *createProcess(char type, int number);

process *createProcessFromString(char *str);

char *process2string(process *p);
