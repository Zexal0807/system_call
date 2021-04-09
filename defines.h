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
#include "err_exit.h"

#ifndef CONSTANT
#define CONSTANT

#define SENDER_FILENAME "output/F8.csv"
#define SENDER_1_FILENAME "output/F1.csv"
#define SENDER_2_FILENAME "output/F2.csv"
#define SENDER_3_FILENAME "output/F3.csv"

#define RECEIVER_FILENAME "output/F9.csv"
#define RECEIVER_1_FILENAME "output/F4.csv"
#define RECEIVER_2_FILENAME "output/F5.csv"
#define RECEIVER_3_FILENAME "output/F6.csv"

#define HACKLER_FILENAME "output/F7_out.csv"
#define MAX_HACKLER_ACTION 10

#define MAX_MESSAGE_LENGTH 150

#define IPC_HISTORY_FILENAME "output/F10.csv"

#define KEY_INIT_SEM 01110011       //115
#define KEY_SHARED_MEMORY 01101101  //109
#define KEY_MESSAGE_QUEUE 01110001  //113
#define KEY_FIFO "tmp/my_fifo.txt"

#define SEM_START 0
#define SEM_INIT_SENDER 1
#define SEM_INIT_RECEIVER 2
#define SEM_END_INIT 3

#define SEM_S1_IS_RUNNNING 4
#define SEM_S2_IS_RUNNNING 5
#define SEM_S3_IS_RUNNNING 6

#define SEM_R1_IS_RUNNNING 7
#define SEM_R2_IS_RUNNNING 8
#define SEM_R3_IS_RUNNNING 9

#define SEM_S1_HAVE_MESSAGE_TO_SEND_BY_PIPE 10
#define SEM_S2_HAVE_MESSAGE_TO_SEND_BY_PIPE 11
#define SEM_R2_HAVE_MESSAGE_TO_SEND_BY_PIPE 12
#define SEM_R3_HAVE_MESSAGE_TO_SEND_BY_PIPE 13

#endif

/*
	Procedura per stampare a console un log di quello che avviene
*/
void printLog(char *p, char *text);

key_t generateKey(int id);