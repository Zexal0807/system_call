/// @file shared_memory.c
/// @brief Contiene l'implementazione delle funzioni specifiche per la gestione della MEMORIA CONDIVISA.

#include "err_exit.h"
#include "shared_memory.h"

/*
int alloc_shared_memory(key_t shmKey, size_t size) {
    int shmid = shmget(shmKey, size, IPC_CREAT | S_IRUSR | S_IWUSR);
    if(shmid == -1){
        errExit("shmget failed");
    }
    return shmid;
}

void *get_shared_memory(int shmid, int shmflg) {
    void *ptr_sh = shmat(shmid, NULL, shmflg);
    if (ptr_sh == (void *)-1)
        errExit("shmat failed");
}

void free_shared_memory(void *ptr_sh) {
    if(shmdt(ptr_sh) == -1){
        errExit("shmdt failed");
    }
}

void remove_shared_memory(int shmid) {
    if(shmctl(shmid, IPC_RMID, NULL) == -1){
        errExit("shmctl failed");
    }
}*/