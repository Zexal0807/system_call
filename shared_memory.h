/// @file shared_memory.h
/// @brief Contiene la definizioni di variabili e funzioni specifiche per la gestione della MEMORIA CONDIVISA.

#pragma once

int createSharedMemory(key_t key);

void * attachSharedMemory(int shmid, int shmflg);

void detachSharedMemory(void *sharedMemory);

void removeSharedMemory(int shmid);

void SH_writeForR1(char * sharedMemory, message * m, int semid);
void SH_writeForR2(char * sharedMemory, message * m, int semid);
void SH_writeForR3(char * sharedMemory, message * m, int semid);