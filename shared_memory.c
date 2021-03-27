/// @file shared_memory.c
/// @brief Contiene l'implementazione delle funzioni specifiche per la gestione della MEMORIA CONDIVISA.

#include "err_exit.h"
#include "shared_memory.h"
#include "struct/message.h"

#include <sys/shm.h>
#include <sys/stat.h>

int createSharedMemory(){
    shmid = shmget(SHARED_MEMORY_KEY, sizeof(message), IPC_CREAT | S_IRUSR | S_IWUSR);
    if(shmid == -1){
        errExit("Impossible create shared memory");
    }
    return shmid;
}

void * attachSharedMemory(int shmid, int shmflg) {
    void *ptr_sh = shmat(shmid, NULL, shmflg);
    if (ptr_sh == (void *)-1)
        errExit("shmat failed");
}

void detachSharedMemory(void *ptr_sh) {
    if(shmdt(ptr_sh) == -1){
        errExit("shmdt failed");
    }
}


void removeSharedMemory(int shmid) {
    if(shmctl(shmid, IPC_RMID, NULL) == -1){
        errExit("shmctl failed");
    }
}