#include "err_exit.h"
#include "message_queue.h"

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <errno.h>

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

    size_t size = sizeof(queueMsg) - sizeof(long);

    if(msgsnd(msqid, &msg, size, 0) == -1){
        ErrExit("Impossible inviare sulla message queue");
    }
}

void sendToR2(int msqid, message *m){
    queueMsg msg;
    msg.mtype = 2;

    char * textMessage = message2line(m);
    memcpy(msg.message, textMessage, strlen(textMessage) + 1);

    size_t size = sizeof(queueMsg) - sizeof(long);

    if(msgsnd(msqid, &msg, size, 0) == -1){
        ErrExit("Impossible inviare sulla message queue");
    }
}

void sendToR3(int msqid, message *m){
    queueMsg msg;
    msg.mtype = 3;

    char * textMessage = message2line(m);
    memcpy(msg.message, textMessage, strlen(textMessage) + 1);

    size_t size = sizeof(queueMsg) - sizeof(long);

    if(msgsnd(msqid, &msg, size, 0) == -1){
        ErrExit("Impossible inviare sulla message queue");
    }
}

char * readR1(int msqid){
    queueMsg msg;
    if (msgrcv(msqid, &msg, 0, 1, IPC_NOWAIT) == -1) {
        if (errno == ENOMSG) {
            printf("No message with type 1 in the queue\n");
        } else {
            ErrExit("msgrcv failed");
        }
    } else {
        return msg.message;
    }
    return "";
}




void deleteMessageQueue(int id){
    if (msgctl(id, IPC_RMID, NULL) == -1)
        ErrExit("msgctl failed");
}