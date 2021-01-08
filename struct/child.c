#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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

int countChildChars(child *c){
	int chars = 0;

	// Type of process
	chars += 1;
	// Number of process
	chars += floor(log10(c->process->number)) + 1;
	// Number of digit of the pid
	chars += floor(log10(c->pid)) + 1;
	// Add the ;
	chars += 2;
	// Add \n
	chars += 1;
	return chars;
}