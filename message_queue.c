#include "err_exit.h"
#include "message_queue.h"

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <errno.h>

size_t queueMsgSize = sizeof(queueMsg) - sizeof(long);

int getMessageQueue(){
    int id = msgget(KEY_MESSAGE_QUEUE, IPC_CREAT | S_IRUSR | S_IWUSR);
    if(id == -1)
        ErrExit("Impossible create message queue");

    return id;
}

void sendToR1(int msqid, message *m){
    queueMsg msg;
    msg.mtype = 1;

    char * textMessage = message2line(m);
    memcpy(msg.message, textMessage, strlen(textMessage) + 1);

    if(msgsnd(msqid, &msg, queueMsgSize, 0) == -1){
        ErrExit("Impossible inviare sulla message queue");
    }
}

void sendToR2(int msqid, message *m){
    queueMsg msg;
    msg.mtype = 2;

    char * textMessage = message2line(m);
    memcpy(msg.message, textMessage, strlen(textMessage) + 1);

    if(msgsnd(msqid, &msg, queueMsgSize, 0) == -1){
        ErrExit("Impossible inviare sulla message queue");
    }
}

void sendToR3(int msqid, message *m){
    queueMsg msg;
    msg.mtype = 3;

    char * textMessage = message2line(m);
    memcpy(msg.message, textMessage, strlen(textMessage) + 1);

    if(msgsnd(msqid, &msg, queueMsgSize, 0) == -1){
        ErrExit("Impossible inviare sulla message queue");
    }
}

message * readR1(int msqid){
    queueMsg msg;
    if (msgrcv(msqid, &msg, queueMsgSize, 1, IPC_NOWAIT) == -1) {
        if (errno != ENOMSG) {
            ErrExit("msgrcv failed");
        }
    } else {
        return line2message(msg.message);
    }
    return NULL;
}

message * readR2(int msqid){
    queueMsg msg;
    if (msgrcv(msqid, &msg, queueMsgSize, 2, IPC_NOWAIT) == -1) {
        if (errno != ENOMSG) {
            ErrExit("msgrcv failed");
        }
    } else {
        return line2message(msg.message);
    }
    return NULL;
}

message * readR3(int msqid){
    queueMsg msg;
    if (msgrcv(msqid, &msg, queueMsgSize, 3, IPC_NOWAIT) == -1) {
        if (errno != ENOMSG) {
            ErrExit("msgrcv failed");
        }
    } else {
        return line2message(msg.message);
    }
    return NULL;
}

void deleteMessageQueue(int id){
    if (msgctl(id, IPC_RMID, NULL) == -1)
        ErrExit("msgctl failed");
}