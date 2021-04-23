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

int createSemaphore(){
    key_t key = generateKey(KEY_INIT_SEM);
    int id = generateSemaphore(key, SEM_NUMBER, S_IRUSR | S_IWUSR | IPC_CREAT);
    return id;
}

void setSemaphore(int semid){
    unsigned short semInitVal[] = {
        3,      // START
        3,      // SM child
        3,      // RM child
        3,      // END INIT
        1,      //S1 is running
        1,      //S2 is running
        1,      //S3 is running
        1,      //R1 is running
        1,      //R2 is running
        1,      //R3 is running
        1,      // S1 have message to send by PIPE
        1,      // S2 have message to send by PIPE
        1,      // R2 have message to send by PIPE
        1,      // R3 have message to send by PIPE
        1,      // Can write on FIFO
        0,      // R1 have message to read in SH
        0,      // R2 have message to read in SH
        0,      // R3 have message to read in SH
        1,      // There is no message in SH
    };
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
        printf("ERR: %d, %d\n", sem_num, sem_op);
        ErrExit("semop failed");
    }
}

int getValue(int semid, int sem_num){
    int value = semctl(semid, sem_num, GETVAL, 0/*ignored*/);
    if (value == -1){
        printf("ERR %d,%d\n", semid, sem_num);
        ErrExit("semctl GETVAL failed");
    }
    return value;
}

void printSemaphoresValue (int semid) {
    unsigned short semVal[SEM_NUMBER];
    union semun arg;
    arg.array = semVal;

    // get the current state of the set
    if (semctl(semid, 0 /*ignored*/, GETALL, arg) == -1)
        ErrExit("semctl GETALL failed");

    // print the semaphore's value
    printf("semaphore set state:\n");
    for (int i = 0; i < SEM_NUMBER; i++)
        printf("id: %d --> %d\n", i, semVal[i]);
}