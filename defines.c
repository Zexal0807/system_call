/// @file defines.c
/// @brief Contiene l'implementazione delle funzioni specifiche di tutto progetto.

#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>

#include "defines.h"

void printLog(char *p, char *text){
    // Print time HH:MM:II
    time_t logtime;
    time(&logtime);
    printf("[%s] ", time_t2string(logtime)); 

    // Print colored process name
	switch(p[0]){
		case 'S':
			if(p[1] == 'M'){
				printf("\033[0;34m");
			}else{
				printf("\033[0;36m");
			}
			break;
		case 'R':
			if(p[1] == 'M'){
				printf("\033[0;31m");
			}else{
				printf("\033[0;35m");
			}
			break;
		case 'H':
			if(p[1] == 'K'){
				printf("\033[0;32m");
			}
			break;
		default:
			printf("\033[0m");
			break;
	}
	printf("%s", p);
	printf("\033[0m");

    // Print action
	printf(" : %s\n", text);
}

key_t generateKey(int idProject){
    // Generate key using ftok
    key_t key = ftok("key.dat", idProject);
    if(key == -1){
        ErrExit("Can't create key");
    }
    return key;
}

int openFile(char * filename){
    // Open a file using open syscall
    int file = open(filename, O_RDONLY);
    if(file == -1){
        ErrExit("Impossibile aprire il file");
    }
    return file;
}
