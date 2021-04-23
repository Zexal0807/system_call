#include "defines.h"
#include "./struct/message.h"

typedef struct {
    long mtype;
    char message[MAX_MESSAGE_LENGTH];
} queueMsg;

int getMessageQueue(key_t key);

void Q_writeForR1(int msqid, message *m);

void Q_writeForR2(int msqid, message *m);

void Q_writeForR3(int msqid, message *m);

/*
    Lettura della messageQueue i messaggi per il corrispondente receiver, return NULL se non trova niente
*/

message * readR1(int msqid);

message * readR2(int msqid);

message * readR3(int msqid);

void deleteMessageQueue(int id);