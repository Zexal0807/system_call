#include <stdlib.h>
#include <stdio.h>

#include "process.h"

process *createProcess(
	char type,
	int number
){

	process *p = (process*) malloc(sizeof(process));

	p->type = type;
	p->number = number;
	
	return p;
}

process *createProcessFromString(char *str){
	return createProcess(str[0], str[1]);
}

char *process2string(process *p){
	char *s = (char*) malloc(sizeof(char) * 2);
	sprintf(s, "%c%d", p->type, p->number);
	return s;
}
