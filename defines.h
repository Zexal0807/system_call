/// @file defines.h
/// @brief Contiene la definizioni di variabili
///         e funzioni specifiche del progetto.

#pragma once

#include <time.h>

#include "struct/process.c"
#include "struct/message.c"
/*
	Procedura per stampare a console un log di quello che avviene
*/
void printLog(char *p, char *text);

/*
Struttura del traffico
	 - message : Messaggio
	 - arrival : Tempo di arrivo
	 - departure : Tempo di partenza
*/
typedef struct {
	message* message;
	time_t arrival;
	time_t departure;
} trafficInfo;

/*
	Funzione per creare un trafficInfo
*/
trafficInfo *createTrafficInfo(
	message* message,
	time_t arrival,
	time_t departure);

/*
	Funzione per convertire un time_t in stringa nel formato HH:MM:SS
*/
char *time_t2string(time_t time);


/*-----------------Hacklet-----------------*/
typedef struct {
	int id;
	int delay;
	process* target;
	int action;
} hackletAction;

hackletAction *createHackletAction(
	int id,
	int delay,
	process* target,
	int action);

char* readFile(char* filename);

void printProcessList(char *filename, char type, process *p1, process *p2, process *p3);

int countTrafficChar(trafficInfo *t);

void printTrafficInfo(char *filename, trafficInfo *data);
