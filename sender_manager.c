/// @file sender_manager.c
/// @brief Contiene l'implementazione del sender_manager.

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char * argv[]) {

	// Check command line input arguments
	if (argc != 2) {
		printf("Error invocation of Sender Manager, you must pass the input file");
		return 1;
	}

	printLog("SM", "Process start");

    int initSemId = createSemaphore();
    setSemaphore(initSemId);

    semOp(initSemId, SEM_START, -1);

    // Wait all process open sem
    semOp(initSemId, SEM_START, 0);

    int pipeS1S2[2];
    int pipeS2S3[2];
    createPipe(pipeS1S2);
    createPipe(pipeS2S3);

    int shmid = createSharedMemory();
    createFIFO();

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
		char string_initSemId[5];
        sprintf(string_initSemId, "%d", initSemId);

        char string_pipes2s3[5];
        sprintf(string_pipes2s3, "%d", pipeS2S3[0]);
        closePipe(pipeS1S2[0]);
        closePipe(pipeS1S2[1]);
        closePipe(pipeS2S3[1]);

        char string_shmId[5];
        sprintf(string_shmId, "%d", shmid);

        char * argv[] = {
			string_initSemId,
            string_pipes2s3,
            string_shmId,
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
        char string_initSemId[5];
        sprintf(string_initSemId, "%d", initSemId);

        char string_pipes1s2[5];
        sprintf(string_pipes1s2, "%d", pipeS1S2[0]);
        closePipe(pipeS1S2[1]);

        char string_pipes2s3[5];
        sprintf(string_pipes2s3, "%d", pipeS2S3[1]);
        closePipe(pipeS2S3[0]);
        
        char string_shmId[5];
        sprintf(string_shmId, "%d", shmid);

        char * argv[] = {
			string_initSemId,
            string_pipes1s2,
            string_pipes2s3,
            string_shmId,
            string_S3pid,
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
        char string_initSemId[5];
        sprintf(string_initSemId, "%d", initSemId);

        char string_pipes1s2[5];
        sprintf(string_pipes1s2, "%d", pipeS1S2[1]);
        closePipe(pipeS1S2[0]);
        closePipe(pipeS2S3[0]);
        closePipe(pipeS2S3[1]);

        char string_shmId[5];
        sprintf(string_shmId, "%d", shmid);

        char string_inputFile[50];
        sprintf(string_inputFile, "%s", argv[1]);
        char * argv[] = {
			string_initSemId,
            string_pipes1s2,
            string_shmId,
            string_inputFile,
            string_S2pid,
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

	printLog("SM", "Process End");
	return 0;
}