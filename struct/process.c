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

process *SENDER_1(){
  return createProcess('S', 1);
}
process *SENDER_2(){
  return createProcess('S', 2);
}
process *SENDER_3(){
  return createProcess('S', 3);
}
process *RECEIVER_1(){
  return createProcess('R', 1);
}
process *RECEIVER_2(){
  return createProcess('R', 2);
}
process *RECEIVER_3(){
  return createProcess('R', 3);
}

process *string2process(char *str){
	return createProcess(str[0], str[1]);
}

char *process2string(process *p){
	if (p->type == 'Z'){
		char *s = (char*) malloc(sizeof(char) * 3);
		sprintf(s, "ALL");
		return s;
	}else{
		char *s = (char*) malloc(sizeof(char) * 2);
		sprintf(s, "%c%d", p->type, p->number);
		return s;
	}
}
