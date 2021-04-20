/// @file shared_memory.h
/// @brief Contiene la definizioni di variabili e funzioni specifiche per la gestione della MEMORIA CONDIVISA.

#pragma once

int createSharedMemory();

void * attachSharedMemory(int shmid, int shmflg);

void detachSharedMemory(void *ptr_sh);

void removeSharedMemory(int shmid);

void SHtoR1(char *ptr_sh, message *m, int semid);
void SHtoR2(message *ptr_sh, message *m, int semid);
void SHtoR3(message *ptr_sh, message *m, int semid);