/// @file fifo.h
/// @brief Contiene la definizioni di variabili e
///			 funzioni specifiche per la gestione delle FIFO.

#pragma once

void createFIFO();

void removeFIFO();

int openSenderFIFO();

int openReceiverFIFO();

void writeFIFO(int fd, char * msg);

void readFIFO(int fd, char * msg);