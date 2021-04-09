/// @file semaphore.h
/// @brief Contiene la definizioni di variabili e funzioni specifiche per la gestione dei SEMAFORI.

#pragma once

union semun {
    int val;
    struct semid_ds * buf;
    unsigned short * array;
};

int generateSemaphore(key_t key, int n, int flags);

int createSemaphore();

void setSemaphore(int semid);

//------------

int createSenderSemaphore();

void setSenderSemaphore(int semid);

int createReceiverSemaphore();

void setReceiverSemaphore(int semid);

//----------------

void removeSemaphore(int semid);

void semOp (int semid, unsigned short sem_num, short sem_op);

int getValue(int semid, int sem_num);

void printSemaphoresValue (int semid);