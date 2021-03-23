/// @file defines.c
/// @brief Contiene l'implementazione delle funzioni specifiche di tutto progetto.

#include "defines.h"

#include <stdio.h>
#include <sys/ipc.h>

void printLog(char *p, char *text){
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
	printf(" : %s\n", text);
}

key_t generateKey(int idProject){
    key_t key = ftok("key.dat", idProject);
    if(key == -1){
        ErrExit("Can't create key");
    }
    return key;
}