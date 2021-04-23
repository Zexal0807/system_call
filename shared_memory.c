/// @file shared_memory.c
/// @brief Contiene l'implementazione delle funzioni specifiche per la gestione della MEMORIA CONDIVISA.

#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "struct/message.h"

int createSharedMemory(key_t key){
    int shmid = shmget(key, sizeof(char)*MAX_MESSAGE_LENGTH, IPC_CREAT | S_IRUSR | S_IWUSR);
    if(shmid == -1){
        ErrExit("Impossible create shared memory");
    }
    return shmid;
}

void * attachSharedMemory(int shmid, int shmflg) {
    void *sharedMemory = shmat(shmid, NULL, shmflg);
    if (sharedMemory == (void *)-1)
        ErrExit("shmat failed");
    return sharedMemory;
}

void detachSharedMemory(void *sharedMemory) {
    if(shmdt(sharedMemory) == -1){
        ErrExit("shmdt failed");
    }
}

void removeSharedMemory(int shmid) {
    if(shmctl(shmid, IPC_RMID, NULL) == -1){
        ErrExit("shmctl failed");
    }
}

void SH_writeForR1(char *sharedMemory, message *m, int semid){
    // Aspetto che la SH sia libera per occuparla
    semOp(semid, SEM_SH, -1);

    // Scrivo il messaggio nella shared memory
    char *msg = message2line(m);
    memcpy(sharedMemory, msg, strlen(msg));

    // Avverto R1 della presenza del messaggio
    semOp(semid, SEM_R1_SH, 1);
}

void SH_writeForR2(char *sharedMemory, message *m, int semid){
    // Aspetto che la SH sia libera per occuparla
    semOp(semid, SEM_SH, -1);

    // Scrivo il messaggio nella shared memory
    char *msg = message2line(m);
    memcpy(sharedMemory, msg, strlen(msg));

    // Avverto R2 della presenza del messaggio
    semOp(semid, SEM_R2_SH, 1);
}

void SH_writeForR3(char *sharedMemory, message *m, int semid){
    // Aspetto che la SH sia libera per occuparla
    semOp(semid, SEM_SH, -1);

    // Scrivo il messaggio nella shared memory
    char *msg = message2line(m);
    memcpy(sharedMemory, msg, strlen(msg));

    // Avverto R3 della presenza del messaggio
    semOp(semid, SEM_R3_SH, 1);
}