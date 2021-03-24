/// @file semaphore.h
/// @brief Contiene la definizioni di variabili e funzioni specifiche per la gestione dei SEMAFORI.

#pragma once

union semun {
    int val;
    struct semid_ds * buf;
    unsigned short * array;
};

int generateSemaphore(key_t key, int n, int flags);

int createInitSemaphore();

void setInitSemaphore(int semid);

void removeSemaphore(int semid);

void semOp (int semid, unsigned short sem_num, short sem_op);

void printSemaphoresValue (int semid);