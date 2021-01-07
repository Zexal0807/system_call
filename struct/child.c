#include <stdlib.h>
#include <stdio.h>

#include "child.h"

child *createChild(
	process *p,
	int pid
){
  
  child *c = (child*) malloc(sizeof(child));

	c->process = p;
	c->pid = pid;
	
	return c;
}
