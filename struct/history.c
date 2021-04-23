#include <stdlib.h>

#include "history.h"

#ifndef IPC_HISTORY_FILE_HEADER
#define IPC_HISTORY_FILE_HEADER "IPC;IDKey;Creator;CreationTime;DestructionTime\n"
#endif

history *createHistory(
    char * ipc, 
    char * key, 
    char * creator, 
    time_t creation, 
    time_t distruction
){

	history *h = (history*) malloc(sizeof(history));

	h->ipc = ipc;
    h->key = key;
    h->creator = creator;
    h->creation = creation;
    h->distruction = distruction;

	return h;
}

int countHistoryChars(history *h){
	int chars = 0;

	// IPC name
	chars += strlen(h->ipc);
    // key name
	chars += strlen(h->key);
    // creator name
	chars += strlen(h->creator);
	// time arrival is HH:MM:SS
	chars += 8;
    // time arrival is HH:MM:SS
	chars += 8;
	// Add the ;
	chars += 4;
	// Add \n
	chars += 1;
	return chars;
}

void printHistorychar *filename, history *data){
	int file;
	if(access(filename, F_OK) == 0){
		// File exist, open in append mode
		file = open(filename, O_WRONLY | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
		ErrOpen(file);
}	else{
		// File not exist, create it, and print the header
		file = open(filename, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
		ErrOpen(file);
		char headerBuffer[] = IPC_HISTORY_FILE_HEADER;
		write(file, headerBuffer, strlen(headerBuffer));
	}

	// Print a line
	int chars = countHistoryChars(data);
	char *buffer  = (char*) malloc(sizeof(char) * chars);
	sprintf(buffer, "%s;%s;%s;%s;%s\n", 
        data->ipc,
        data->key,
        data->creator,
		time_t2string(data->creation),
		time_t2string(data->distruction)
	);
	write(file, buffer, strlen(buffer));

	// Close file
	close(file);
}