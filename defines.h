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

#define KEY_INIT_SEM 01110011       //S
#define KEY_SENDER_SEM 2
#define KEY_PIPE_S1_S2 3
#define KEY_PIPE_S2_S3 4
#define KEY_RECEIVER_SEM 5
#define KEY_PIPE_R1_R2 6
#define KEY_PIPE_R2_R3 7
#define KEY_SHARED_MEMORY 01101101  //M
#define KEY_MESSAGE_QUEUE 01110001  //Q
#define KEY_FIFO "output/my_fifo.txt"

#endif

/*
	Procedura per stampare a console un log di quello che avviene
*/
void printLog(char *p, char *text);

key_t generateKey(int id);