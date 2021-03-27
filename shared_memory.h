/// @file shared_memory.h
/// @brief Contiene la definizioni di variabili e funzioni specifiche per la gestione della MEMORIA CONDIVISA.

#pragma once

int createSharedMemory();

void * attachSharedMemory(int shmid, int shmflg);

void detachSharedMemory(void *ptr_sh);

void removeSharedMemory(int shmid);