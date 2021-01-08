/// @file defines.h
/// @brief Contiene la definizioni di variabili
///         e funzioni specifiche del progetto.

#pragma once

#include "struct/process.h"
#include "struct/child.h"
#include "struct/message.h"
#include "struct/trafficInfo.h"
#include "struct/hacklerAction.h"
#include "struct/list.h"

/*
	Procedura per stampare a console un log di quello che avviene
*/
void printLog(char *p, char *text);

/*
	Funzione per convertire un time_t in stringa nel formato HH:MM:SS
*/
char *time_t2string(time_t time);

/*
	Funzione per stampare un child su file
*/
void printChild(char *filename, child *data);

/*
	Funzione per trafficInfo un child su file
*/
void printTrafficInfo(char *filename, trafficInfo *data);

/*
	Funzione per stampare un hackletAction su file
*/
void printHacklerAction(char *filenam, hackletAction *data);
