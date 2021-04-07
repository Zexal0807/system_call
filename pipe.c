/// @file pipe.c
/// @brief Contiene l'implementazione delle funzioni
///         specifiche per la gestione delle PIPE.

#include "err_exit.h"
#include "pipe.h"

#include <unistd.h>

void createPipe (int fd[]){
    if (pipe(fd) == -1)
        ErrExit("PIPE");
}

void closePipe(int fd){
    if(close(fd) == -1)
        ErrExit("Impossible close pipe");
}
