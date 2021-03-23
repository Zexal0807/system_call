/// @file semaphore.h
/// @brief Contiene la definizioni di variabili e funzioni specifiche per la gestione dei SEMAFORI.

#pragma once
/*
struct sembuf {
    unsigned short sem_num;
    short sem_op;
    short sem_flg;
};
*/
int generateSemaphore(key_t key, int n, int flags);

int createInitSemaphore();
int getInitSemaphore();

void semOp (int semid, unsigned short sem_num, short sem_op);
