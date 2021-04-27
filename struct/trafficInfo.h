#pragma once

#include <time.h>

#include "message.h"

/*
Struttura del traffico
	 - message : Messaggio
	 - arrival : Tempo di arrivo
	 - departure : Tempo di partenza
*/
typedef struct {
	message * message;
	time_t arrival;
	time_t departure;
} trafficInfo;

/*
	Funzione per creare un trafficInfo
*/
trafficInfo * createTrafficInfo(message * message, time_t arrival, time_t departure);

/*
	Funzione per convertire un time_t in stringa nel formato HH:MM:SS
*/
char * time_t2string(time_t time);

/*
	Crea il file se non essite, troncalo se esite e stampa gli header
*/
void printHeader(char * filename);

/*
	Funzione per trafficInfo un child su file
*/
void printTrafficInfo(char * filename, trafficInfo * data);
