/// @file fifo.c
/// @brief Contiene l'implementazione delle funzioni specifiche per la gestione delle FIFO.

#include "err_exit.h"
#include "fifo.h"
#include "defines.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void createFIFO(){
    if(mkfifo(KEY_FIFO, S_IRUSR|S_IWUSR) == -1){
        ErrExit("Impossible create fifo");
    }
}

int openSenderFIFO(){
    int fd = open(KEY_FIFO, O_WRONLY);
    if(fd == -1){
        ErrExit("Impossible open fifo as sender");
    }
    return fd;
}

int openReceiverFIFO(){
    int fd = open(KEY_FIFO, O_RDONLY);
    if(fd == -1){
        ErrExit("Impossible open fifo as receiver");
    }
    return fd;
}