#include <stdlib.h>
#include <stdio.h>

#include "child.h"

child *createChild(
	char type,
	int number,
	int pid
){
  
  child *c = (child*) malloc(sizeof(child));
	process *p = createProcess(type, number);

	c->process = p;
	c->pid = pid;
	
	return c;
}
