/// @file receiver_manager.c
/// @brief Contiene l'implementazione del receiver_manager.
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"

int main(int argc, char * argv[]) {

	// Check command line input arguments
	if (argc != 1) {
		printf("Error invocation of Receiver Manager");
		return 1;
	}

	printLog("RM", "Process start");

    int initSemId = createSemaphore();
    semOp(initSemId, SEM_START, -1);
    
    // Wait all process open sem
    semOp(initSemId, SEM_START, 0);

    int pipeR1R2[2];
    int pipeR2R3[2];
    createPipe(pipeR1R2);
    createPipe(pipeR2R3);

    int shmid = createSharedMemory();

	// Define the 3 struct process
	child * R1 = NULL;
	child * R2 = NULL;
	child * R3 = NULL;

	// Try to create a child, in each child functione must me an exit
	int pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork R1");
	} else if (pid == 0) {
        char string_initSemId[5];
        sprintf(string_initSemId, "%d", initSemId);

        char string_piper1r2[5];
        sprintf(string_piper1r2, "%d", pipeR1R2[0]);
        closePipe(pipeR1R2[1]);
        closePipe(pipeR2R3[0]);
        closePipe(pipeR2R3[1]);

        char string_shmId[5];
        sprintf(string_shmId, "%d", shmid);

        char * argv[] = {
			string_initSemId,
            string_piper1r2,
            string_shmId,
            NULL
		};
		execvp("./R1", argv);
		ErrExit("R1 not start");
	}
	printLog("RM", "R1 start");
	R1 = createChild(RECEIVER_1(), pid);
	printChild(RECEIVER_FILENAME, R1);

    char string_R1pid[5];
    sprintf(string_R1pid, "%d", pid);

	// Try to create a child, in each child functione must me an exit
	pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork R2");
	} else if (pid == 0) {
        char string_initSemId[5];
        sprintf(string_initSemId, "%d", initSemId);

        char string_piper1r2[5];
        sprintf(string_piper1r2, "%d", pipeR1R2[1]);
        closePipe(pipeR1R2[0]);
        char string_piper2r3[5];
        sprintf(string_piper2r3, "%d", pipeR2R3[0]);
        closePipe(pipeR2R3[1]);

        char string_shmId[5];
        sprintf(string_shmId, "%d", shmid);

        char * argv[] = {
			string_initSemId,
            string_piper1r2,
            string_piper2r3,
            string_shmId,
            string_R1pid,
            NULL
		};

		execvp("./R2", argv);
		ErrExit("R2 not start");
	}
	printLog("RM", "R2 start");
	R2 = createChild(RECEIVER_2(), pid);
	printChild(RECEIVER_FILENAME, R2);

    char string_R2pid[5];
    sprintf(string_R2pid, "%d", pid);

	// Try to create a child, in each child functione must me an exit
	pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork R3");
	} else if (pid == 0) {
		char string_initSemId[5];
        sprintf(string_initSemId, "%d", initSemId);
        
        char string_piper2r3[5];
        sprintf(string_piper2r3, "%d", pipeR2R3[1]);
        closePipe(pipeR1R2[0]);
        closePipe(pipeR1R2[1]);
        closePipe(pipeR2R3[0]);

        char string_shmId[5];
        sprintf(string_shmId, "%d", shmid);

        char * argv[] = {
			string_initSemId,
            string_piper2r3,
            string_shmId,
            string_R2pid,
            NULL
		};
		execvp("./R3", argv);
		ErrExit("R3 not start");
	}
	printLog("RM", "R3 start");
	R3 = createChild(RECEIVER_3(), pid);
	printChild(RECEIVER_FILENAME, R3);

    // Wait all child init end
    semOp(initSemId, SEM_INIT_RECEIVER, 0);

    // Set this process as end init     
    semOp(initSemId, SEM_END_INIT, -1);

    // Wait all init end
    semOp(initSemId, SEM_END_INIT, 0);

    printLog("RM", "End init start");

	// Wait the end of all child
	pid_t child;
	int status;
	while ((child = wait( & status)) != -1) {
		//printf("returned child %d with status %d\n", child, status);
	}

	printLog("RM", "Process End");
	return 0;
}