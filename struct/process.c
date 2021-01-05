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

process *createProcess(
	char type,
	int number,
	int pid){

    process *p = (process*) malloc(sizeof(process));

    p->type = type;
    p->number = number;
    p->pid = pid;
    
    return p;
}

char *process2string(process *p){
    char *s = (char*) malloc(sizeof(char) * 2);
    sprintf(s, "%c%d", p->type, p->number);
    return s;
}
