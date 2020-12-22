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
