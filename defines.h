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

#define SENDER_FILENAME "OutputFiles/F8.csv"
#define SENDER_1_FILENAME "OutputFiles/F1.csv"
#define SENDER_2_FILENAME "OutputFiles/F2.csv"
#define SENDER_3_FILENAME "OutputFiles/F3.csv"

#define RECEIVER_FILENAME "OutputFiles/F9.csv"
#define RECEIVER_1_FILENAME "OutputFiles/F4.csv"
#define RECEIVER_2_FILENAME "OutputFiles/F5.csv"
#define RECEIVER_3_FILENAME "OutputFiles/F6.csv"

#define HACKLER_FILENAME "OutputFiles/F7_out.csv"
#define MAX_HACKLER_ACTION 20

#define HK_ACTION_INCREASE_DELAY "IncreaseDelay"
#define HK_ACTION_REMOVE_MSG "RemoveMSG"
#define HK_ACTION_SEND_MSG "SendMSG"
#define HK_ACTION_SHUT_DOWN "ShutDown"

#define MAX_MESSAGE_LENGTH 150

#define IPC_HISTORY_FILENAME "OutputFiles/F10.csv"

#define KEY_INIT_SEM 01110011       // S
#define KEY_SHARED_MEMORY 01101101  // M
#define KEY_MESSAGE_QUEUE 01110001  // Q
#define KEY_FIFO "OutputFiles/my_fifo.txt"

#define SEM_NUMBER 20

#define SEM_START 0
#define SEM_INIT_SENDER 1
#define SEM_INIT_RECEIVER 2
#define SEM_END_INIT 3

#define SEM_S1_IS_RUNNING 4
#define SEM_S2_IS_RUNNING 5
#define SEM_S3_IS_RUNNING 6

#define SEM_R1_IS_RUNNING 7
#define SEM_R2_IS_RUNNING 8
#define SEM_R3_IS_RUNNING 9

#define SEM_S1_HAVE_MESSAGE_TO_SEND_BY_PIPE 10
#define SEM_S2_HAVE_MESSAGE_TO_SEND_BY_PIPE 11
#define SEM_R2_HAVE_MESSAGE_TO_SEND_BY_PIPE 12
#define SEM_R3_HAVE_MESSAGE_TO_SEND_BY_PIPE 13

#define SEM_MESSAGE_IN_FIFO 14

#define SEM_R1_SH 15
#define SEM_R2_SH 16
#define SEM_R3_SH 17

#define SEM_SH 18

#define SEM_HISTORY_FILE 19

#endif

/*
	Procedura per stampare a console un log di quello che avviene
*/
void printLog(char *p, char *text);

/*
    Funzione per generare una chiave IPC
*/
key_t generateKey(int id);

/*
    Open a file like fopen
*/
int openFile(char * filename);

char *time_t2string(time_t time);