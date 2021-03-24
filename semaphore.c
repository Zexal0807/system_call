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
    int id = generateSemaphore(key, 5, S_IRUSR | S_IWUSR | IPC_CREAT);
    return id;
}

void setInitSemaphore(int semid){
    // START, SM child, RM child, HK, END INIT
    unsigned short semInitVal[] = {3, 3, 3, 1, 3};
    union semun arg;
    arg.array = semInitVal;

    if (semctl(semid, 0, SETALL, arg) == 1)
        ErrExit("semctl SETALL failed");
}

void removeSemaphore(int semid){
    if (semctl(semid, 0, IPC_RMID, 0) == -1)
        ErrExit("semctl failed");
}

void semOp (int semid, unsigned short sem_num, short sem_op) {
    struct sembuf sop = { 
      sem_num,
      sem_op,
      0
    };
    
    if (semop(semid, &sop, 1) == -1){
        printf("ERRORE semOP: %d, %d\n", sem_num, sem_op);
        ErrExit("semop failed");
    }
}

void printSemaphoresValue (int semid) {
    unsigned short semVal[5];
    union semun arg;
    arg.array = semVal;

    // get the current state of the set
    if (semctl(semid, 0 /*ignored*/, GETALL, arg) == -1)
        ErrExit("semctl GETALL failed");

    // print the semaphore's value
    printf("semaphore set state:\n");
    for (int i = 0; i < 5; i++)
        printf("id: %d --> %d\n", i, semVal[i]);
}