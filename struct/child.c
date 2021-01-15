#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "child.h"
#include "../err_exit.h"

#ifndef CHILD_FILE_HEADER
#define CHILD_FILE_HEADER "Id;Pid\n"
#endif

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

void printChild(char *filename, child *data){
	int file;
	if(access(filename, F_OK) == 0){
		// File exist, open in append mode
		file = open(filename, O_WRONLY | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
		ErrOpen(file);
	}else{
		// File not exist, create it, and print the header
		file = open(filename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
		ErrOpen(file);
		char headerBuffer[] = CHILD_FILE_HEADER;
		write(file, headerBuffer, strlen(headerBuffer));
	}

	// Print a line
	int chars = countChildChars(data);
	char *buffer  = (char*) malloc(sizeof(char) * chars);
	sprintf(buffer, "%s;%d\n", 
		process2string(data->process),
		data->pid
	);
	write(file, buffer, strlen(buffer));

	// Close file
	close(file);
}
