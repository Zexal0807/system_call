
#include "err_exit.h"
#include "message_queue.h"
#include "defines.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <errno.h>

size_t queueMsgSize = sizeof(queueMsg) - sizeof(long);

int getMessageQueue(key_t key){
	int id = msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR);
	if(id == -1)
		ErrExit("Impossible create message queue");

	return id;
}

void Q_write(int msqid, message * m, long mtype){
	queueMsg msg;
	msg.mtype = mtype;

	char * textMessage = message2line(m);
	memcpy(msg.message, textMessage, strlen(textMessage) + 1);

	if(msgsnd(msqid, &msg, queueMsgSize, 0) == -1){
		ErrExit("Impossible inviare sulla message queue");
	}
}

void Q_writeForR1(int msqid, message * m){
	Q_write(msqid, m, 1);
}

void Q_writeForR2(int msqid, message * m){
	Q_write(msqid, m, 2);
}

void Q_writeForR3(int msqid, message *m){
	Q_write(msqid, m, 3);
}

message * read(int msqid, long mtype){
	queueMsg msg;
	if (msgrcv(msqid, &msg, queueMsgSize, mtype, IPC_NOWAIT) == -1) {
		if (errno != ENOMSG) {
			ErrExit("msgrcv failed");
		}
	} else {
		return line2message(msg.message);
	}
	return NULL;
}

message * readR1(int msqid){
	return read(msqid, 1);
}

message * readR2(int msqid){
	return read(msqid, 2);
}

message * readR3(int msqid){
	return read(msqid, 3);
}

void deleteMessageQueue(int id){
	if (msgctl(id, IPC_RMID, NULL) == -1)
		ErrExit("msgctl failed");
}