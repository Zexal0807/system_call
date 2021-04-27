/// @file err_exit.c
/// @brief Contiene l'implementazione della funzione di stampa degli errori.

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>

#include "err_exit.h"

void ErrExit(const char * msg){
	perror(msg);
	exit(1);
}

void ErrOpen(int file){
	if(file == -1){
		ErrExit("File not found");
	}
}