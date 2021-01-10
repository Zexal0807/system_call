#include <stdlib.h>
#include <stdio.h>

#include "process.h"
#include "../err_exit.h"

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
	if(str[0] == 'A' || str[0] == 'a'){
		if(str[1] == 'L' || str[1] == 'l'){
			if(str[2] == 'L' || str[2] == 'l'){
				return createProcess('Z', 0);
			}	
		}
	}else{
		int number = atoi(&str[1]);
		if(str[0] == 'S' || str[0] == 's'){
			return createProcess('S', number);
		}else if(str[0] == 'R' || str[0] == 'r'){
			return createProcess('R', number);
		}
	}
	ErrExit("Error can't convert string to process\n");
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
