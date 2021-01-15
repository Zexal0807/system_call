/// @file receiver_manager.c
/// @brief Contiene l'implementazione del receiver_manager.

#include "err_exit.h"
#include "defines.h"
#include "shared_memory.h"
#include "semaphore.h"
#include "fifo.h"
#include "pipe.h"

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char * argv[]) {

	// Check command line input arguments
	if (argc != 1) {
		printf("Error invocation of Receiver Manager");
		return 1;
	}

	printLog("RM", "Process start");

	// Define the 3 struct process
	child * R1 = NULL;
	child * R2 = NULL;
	child * R3 = NULL;

	// Try to create a child, in each child functione must me an exit
	int pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork R1");
	} else if (pid == 0) {
		char * argv[] = {
			NULL
		};
		execvp("./R1", argv);
		ErrExit("R1 not start");
	}
	printLog("RM", "R1 start");
	R1 = createChild(RECEIVER_1(), pid);
	printChild(RECEIVER_FILENAME, R1);

	// Try to create a child, in each child functione must me an exit
	pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork R2");
	} else if (pid == 0) {
		char * argv[] = {
			NULL
		};
		execvp("./R2", argv);
		ErrExit("R2 not start");
	}
	printLog("RM", "R2 start");
	R2 = createChild(RECEIVER_2(), pid);
	printChild(RECEIVER_FILENAME, R2);

	// Try to create a child, in each child functione must me an exit
	pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork R3");
	} else if (pid == 0) {
		char * argv[] = {
			NULL
		};
		execvp("./R3", argv);
		ErrExit("R3 not start");
	}
	printLog("RM", "R3 start");
	R3 = createChild(RECEIVER_3(), pid);
	printChild(RECEIVER_FILENAME, R3);
	
	// Wait the end of all child
	pid_t child;
	int status;
	while ((child = wait( & status)) != -1) {
		//printf("returned child %d with status %d\n", child, status);
	}

	printLog("RM", "Process End");
	return 0;
}