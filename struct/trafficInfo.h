#pragma once

#include "message.h"

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
