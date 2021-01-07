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
	R1 = createChild('R', 1, pid);

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
	R2 = createChild('R', 2, pid);

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
	R3 = createChild('R', 3, pid);

	//Save process pid in file
	printChildList("output/F9.csv", 'R', R1, R2, R3);

	// Wait the end of all child
	pid_t child;
	int status;
	while ((child = wait( & status)) != -1) {
		//printf("returned child %d with status %d\n", child, status);
	}

	printLog("RM", "Process End");
	return 0;
}