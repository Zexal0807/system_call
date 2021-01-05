#include <stdlib.h>
#include <stdio.h>

#include "process.h"

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

process *createProcessFromString(char *str){
  return createProcess(str[0], str[1], 0);
}

char *process2string(process *p){
	char *s = (char*) malloc(sizeof(char) * 2);
	sprintf(s, "%c%d", p->type, p->number);
	return s;
}
