#include "defines.h"
#include "./struct/message.h"

typedef struct {
    long mtype;
    char message[MAX_MESSAGE_LENGTH];
} queueMsg;

int getMessageQueue();

void sendToR1(int msqid, message *m);

void sendToR2(int msqid, message *m);

void sendToR3(int msqid, message *m);

message * readR1(int msqid);

message * readR2(int msqid);

message * readR3(int msqid);

void deleteMessageQueue(int id);
