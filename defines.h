/// @file defines.h
/// @brief Contiene la definizioni di variabili
///         e funzioni specifiche del progetto.

#pragma once

/*
	Struttura di un processo
	 - type : Tipo del processo [S: Sender, R: Reciver]
	 - number : Numero del processo
	
	Target Z0 : ALL
*/
typedef struct {
	char type;
	int number;
} process;

/*
	Struttura del messaggio
	 - id : Identificativo del messaggio
	 - content : Stringa del messsaggio
	 - sender : Processo che deve spedire il messaggio
	 - reciver : Pocesso che deve riceve il messsaggio
	 - delay1 : Tempo di attessa in S1 e R1
	 - delay2 : Tempo di attessa in S2 e R2
	 - delay3 : Tempo di attessa in S3 e R3
	 - comunication : Tipo di comunciazione [1 : MessageQueue, 2 : SharedMemory, 3 : FIFO]
*/
typedef struct {
	int id;
	char *content;
	process sender;
	process reciver;
	int delay1;
	int delay2;
	int delay3;
	int comunication;
} message;

message *createMessage(
	int id, 
	char* message, 
	process sender, 
	process reciver, 
	int delay1, 
	int delay2, 
	int delay3, 
	int comunication);
