/// @file sender_manager.c
/// @brief Contiene l'implementazione del sender_manager.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "message_queue.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"
#include "struct/history.h"

history * PIPES1S2;
history * PIPES2S3;
history * SEM;
history * SH;
history * MSGQUEUE;
history * FIFO;

void printIPCHistory(int semId, history * h){
	
	time_t timeIPC;

	// Set distruction time
	time(&timeIPC);
	h->distruction = timeIPC;

	// Wait can write on file
	semOp(semId, SEM_HISTORY_FILE, -1);

	// Write
	printHistory(IPC_HISTORY_FILENAME, h);

	// Free file
	semOp(semId, SEM_HISTORY_FILE, 1);
}

int main(int argc, char * argv[]) {

	time_t timeIPC;

	// Check command line input arguments
	if (argc != 2) {
		printf("Error invocation of Sender Manager, you must pass the input file");
		return 1;
	}

	printLog("SM", "Process start");
	
	key_t key;
	char charKey[10];

	key = generateKey(KEY_INIT_SEM);
	int initSemId = createSemaphore(key);
	sprintf(charKey, "%x", key);
	time(&timeIPC);
	SEM = createHistory("SEM", charKey,  "SM", timeIPC, timeIPC);
	setSemaphore(initSemId);

	semOp(initSemId, SEM_START, -1);

	// Wait all process open sem
	semOp(initSemId, SEM_START, 0);

	// Create PIPEs
	int pipeS1S2[2];
	createPipe(pipeS1S2);
	sprintf(charKey, "%d/%d", pipeS1S2[0], pipeS1S2[1]);
	time(&timeIPC);
	PIPES1S2 = createHistory("PIPES1S2", charKey,  "SM", timeIPC, timeIPC);
	
	int pipeS2S3[2];
	createPipe(pipeS2S3);
	sprintf(charKey, "%d/%d", pipeS2S3[0], pipeS2S3[1]);
	time(&timeIPC);
	PIPES2S3 = createHistory("PIPES2S3", charKey,  "SM", timeIPC, timeIPC);

	// Create SH
	key = generateKey(KEY_SHARED_MEMORY);
	sprintf(charKey, "%x", key);
	int shmid = createSharedMemory(key);
	time(&timeIPC);
	SH = createHistory("SH", charKey,  "SM", timeIPC, timeIPC);

	// Create FIFO
	createFIFO();
	time(&timeIPC);
	FIFO = createHistory("FIFO", "-",  "SM", timeIPC, timeIPC);

	// Create MSGQueue
	key = generateKey(KEY_MESSAGE_QUEUE);
	sprintf(charKey, "%x", key);
	int messageQueueId = getMessageQueue(key);
	time(&timeIPC);
	MSGQUEUE = createHistory("MQ", charKey,  "SM", timeIPC, timeIPC);

	// Define the 3 struct process
	child *S1 = NULL;
	child *S2 = NULL;
	child *S3 = NULL;

	int pid;

	// Try to create a child, in each child functione must me an exit
	pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork S3");
	} else if (pid == 0) {
		char string_initSemId[15];
		sprintf(string_initSemId, "%d", initSemId);

		char string_pipes2s3[15];
		sprintf(string_pipes2s3, "%d", pipeS2S3[0]);
		closePipe(pipeS1S2[0]);
		closePipe(pipeS1S2[1]);
		closePipe(pipeS2S3[1]);

		char string_shmId[15];
		sprintf(string_shmId, "%d", shmid);

		char string_messageQueueId[15];
		sprintf(string_messageQueueId, "%d", messageQueueId);

		char * argv[] = {
			string_initSemId,
			string_pipes2s3,
			string_shmId,
			string_messageQueueId,
			NULL
		};
		execvp("./S3", argv);
		ErrExit("S3 not start");
	}
	printLog("SM", "S3 start");
	S3 = createChild(SENDER_3(), pid);

	char string_S3pid[5];
	sprintf(string_S3pid, "%d", pid);

	// Try to create a child, in each child functione must me an exit
	pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork S2");
	} else if (pid == 0) {
		char string_initSemId[15];
		sprintf(string_initSemId, "%d", initSemId);

		char string_pipes1s2[15];
		sprintf(string_pipes1s2, "%d", pipeS1S2[0]);
		closePipe(pipeS1S2[1]);

		char string_pipes2s3[15];
		sprintf(string_pipes2s3, "%d", pipeS2S3[1]);
		closePipe(pipeS2S3[0]);
		
		char string_shmId[15];
		sprintf(string_shmId, "%d", shmid);

		char string_messageQueueId[15];
		sprintf(string_messageQueueId, "%d", messageQueueId);

		char * argv[] = {
			string_initSemId,
			string_pipes1s2,
			string_pipes2s3,
			string_shmId,
			string_S3pid,
			string_messageQueueId,
			NULL
		};
		execvp("./S2", argv);
		ErrExit("S2 not start");
	}
	printLog("SM", "S2 start");
	S2 = createChild(SENDER_2(), pid);

	char string_S2pid[5];
	sprintf(string_S2pid, "%d", pid);

	// Try to create a child, in each child functione must me an exit
	pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork S1");
	} else if (pid == 0) {
		char string_initSemId[15];
		sprintf(string_initSemId, "%d", initSemId);

		char string_pipes1s2[15];
		sprintf(string_pipes1s2, "%d", pipeS1S2[1]);
		closePipe(pipeS1S2[0]);
		closePipe(pipeS2S3[0]);
		closePipe(pipeS2S3[1]);

		char string_shmId[15];
		sprintf(string_shmId, "%d", shmid);
		
		char string_messageQueueId[15];
		sprintf(string_messageQueueId, "%d", messageQueueId);

		char string_inputFile[50];
		sprintf(string_inputFile, "%s", argv[1]);
		char * argv[] = {
			string_initSemId,
			string_pipes1s2,
			string_shmId,
			string_S2pid,
			string_messageQueueId,
			string_inputFile,
			NULL
		};
		execvp("./S1", argv);
		ErrExit("S1 not start");
	}
	printLog("SM", "S1 start");
	S1 = createChild(SENDER_1(), pid);

	printChild(SENDER_FILENAME, S1);
	printChild(SENDER_FILENAME, S2);
	printChild(SENDER_FILENAME, S3);

	// Wait all child init end
	semOp(initSemId, SEM_INIT_SENDER, 0);

	printLog("SM", "End init of all child");

	// Set this process as end init
	semOp(initSemId, SEM_END_INIT, -1);

	printLog("SM", "End init start");

	// Wait all init end
	semOp(initSemId, SEM_END_INIT, 0);

	// Wait the end of all child
	pid_t child;
	int status;
	while ((child = wait( &status)) != -1) {
		//printf("returned child %d with status %d\n", child, status);
	}

	// Wait RM end
	semOp(initSemId, SEM_R1_IS_RUNNING, 0);
	semOp(initSemId, SEM_R2_IS_RUNNING, 0);
	semOp(initSemId, SEM_R3_IS_RUNNING, 0);

	// Close PIPEs
	closePipe(pipeS1S2[0]);
	closePipe(pipeS1S2[1]);
	printIPCHistory(initSemId, PIPES1S2);

	closePipe(pipeS2S3[0]);
	closePipe(pipeS2S3[1]);
	printIPCHistory(initSemId, PIPES2S3);
	
	removeFIFO();

	// Remove IPC
	deleteMessageQueue(messageQueueId);
	printIPCHistory(initSemId, MSGQUEUE);

	removeSharedMemory(shmid);
	printIPCHistory(initSemId, SH);

	// Print before write for use the same function
	semOp(initSemId, SEM_RM_IS_RUNNING, 0);
	semOp(initSemId, SEM_HK_IS_RUNNING, 0);
	printIPCHistory(initSemId, SEM);
	removeSemaphore(initSemId);

	printLog("SM", "Process End");
	return 0;
}