/// @file defines.h
/// @brief Contiene la definizioni di variabili
///         e funzioni specifiche del progetto.

#pragma once

#include <time.h>

void printLog(char *p, char *text);

/*
	Struttura di un processo
	 - type : Tipo del processo [S: Sender, R: Reciver]
	 - number : Numero del processo
	 - pid : PID del processo
	Target Z0 : ALL
*/
typedef struct {
	char type;
	int number;
	int pid;
} process;

process *createProcess(
	char type,
	int number,
	int pid);

/*
	Struttura del messaggio
	 - id : Identificativo del messaggio
	 - content : Stringa del messsaggio
	 - sender : Processo che deve spedire il messaggio
	 - reciver : Pocesso che deve riceve il messsaggio
	 - delay1 : Tempo di attessa in S1 e R1
	 - delay2 : Tempo di attessa in S2 e R2
	 - delay3 : Tempo di attessa in S3 e R3
	 - comunication : Tipo di comunciazione
*/
typedef struct {
	int id;
	char *content;
	process *sender;
	process *receiver;
	int delay1;
	int delay2;
	int delay3;
	char* comunication;
} message;

message *createMessage(
	int id, 
	char* message, 
	process* sender, 
	process* receiver, 
	int delay1, 
	int delay2, 
	int delay3, 
	char* comunication);

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

trafficInfo *createTrafficInfo(
	message* message,
	time_t arrival,
	time_t departure);

char *time_t2string(time_t time);

char *process2string(process *p);

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

void printTrafficInfo(char *filename, trafficInfo *data);