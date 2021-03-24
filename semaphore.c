/// @file semaphore.c
/// @brief Contiene l'implementazione delle funzioni specifiche per la gestione dei SEMAFORI.
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/sem.h>

#include "err_exit.h"
#include "semaphore.h"
#include "defines.h"

int generateSemaphore(key_t key, int n, int flags){
    int id = semget(key, n, flags);
    if (id == -1){
        ErrExit("semget failed");
    }
    return id;
}

int createInitSemaphore(){
    key_t key = generateKey(KEY_INIT_SEM);
    int id = generateSemaphore(key, 4, S_IRUSR | S_IWUSR | IPC_CREAT);
    return id;
}

void setInitSemaphore(semid){
    unsigned short semInitVal[] = {3, 3, 1, 3};
    union semun arg;
    arg.array = semInitVal;

    if (semctl(semid, 0, SETALL, semInitVal) == 1)
        ErrExit("Init semctl SETALL failed");
}

int getInitSemaphore(){
    key_t key = generateKey(KEY_INIT_SEM);
    int id = generateSemaphore(key, 4, S_IRUSR | S_IWUSR );
    return id;
}

void semOp (int semid, unsigned short sem_num, short sem_op) {
    struct sembuf sop = { 
      sem_num,
      sem_op,
      0
    };

    if (semop(semid, &sop, 1) == -1)
        ErrExit("semop failed");
}

