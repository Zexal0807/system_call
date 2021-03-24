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

    int initSemId = createInitSemaphore();
    setInitSemaphore(initSemId);
    semOp(initSemId, 0, -1);

    // Wait all process open sem
    semOp(initSemId, 0, 0);

	// Define the 3 struct process
	child *S1 = NULL;
	child *S2 = NULL;
	child *S3 = NULL;

	// Try to create a child, in each child functione must me an exit
	int pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork S1");
	} else if (pid == 0) {
        char string_initSemId[5];
        sprintf(string_initSemId, "%d", initSemId);
        char string_inputFile[50];
        sprintf(string_inputFile, "%s", argv[1]);
        char * argv[] = {
			string_initSemId,
            string_inputFile,
            NULL
		};
		execvp("./S1", argv);
		ErrExit("S1 not start");
	}
    printLog("SM", "S1 start");
	S1 = createChild(SENDER_1(), pid);
	printChild(SENDER_FILENAME, S1);

	// Try to create a child, in each child functione must me an exit
	pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork S2");
	} else if (pid == 0) {
        char string_initSemId[5];
        sprintf(string_initSemId, "%d", initSemId);
        char * argv[] = {
			string_initSemId,
            NULL
		};
		execvp("./S2", argv);
		ErrExit("S2 not start");
	}
    printLog("SM", "S2 start");
	S2 = createChild(SENDER_2(), pid);
	printChild(SENDER_FILENAME, S2);

	// Try to create a child, in each child functione must me an exit
	pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork S3");
	} else if (pid == 0) {
		char string_initSemId[5];
        sprintf(string_initSemId, "%d", initSemId);
        char * argv[] = {
			string_initSemId,
            NULL
		};
		execvp("./S3", argv);
		ErrExit("S3 not start");
	}
    printLog("SM", "S3 start");
	S3 = createChild(SENDER_3(), pid);
	printChild(SENDER_FILENAME, S3);

	// Wait all child init end
    semOp(initSemId, 1, 0);

    // Set this process as end init     
    semOp(initSemId, 4, -1);

    // Wait all init end
    semOp(initSemId, 4, 0);

	// Wait the end of all child
	pid_t child;
	int status;
	while ((child = wait( & status)) != -1) {
		//printf("returned child %d with status %d\n", child, status);
	}

    // Remove used IPC
    removeSemaphore(initSemId);

	printLog("SM", "Process End");
	return 0;
}