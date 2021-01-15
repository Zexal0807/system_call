/// @file sender_manager.c
/// @brief Contiene l'implementazione del sender_manager.

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
	if (argc != 2) {
		printf("Error invocation of Sender Manager, you must pass the input file");
		return 1;
	}

	printLog("SM", "Process start");

	// Define the 3 struct process
	child *S1 = NULL;
	child *S2 = NULL;
	child *S3 = NULL;

	// Try to create a child, in each child functione must me an exit
	int pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork S1");
	} else if (pid == 0) {
		execvp("./S1", & argv[1]);
		ErrExit("S1 not start");
	}
	S1 = createChild(SENDER_1(), pid);
	printChild(SENDER_FILENAME, S1);

	// Try to create a child, in each child functione must me an exit
	pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork S2");
	} else if (pid == 0) {
		char * argv[] = {
			NULL				
		};			
		execvp("./S2", argv);
		ErrExit("S2 not start");
	}
	S2 = createChild(SENDER_2(), pid);
	printChild(SENDER_FILENAME, S2);

	// Try to create a child, in each child functione must me an exit
	pid = fork();
	if (pid == -1) {
		ErrExit("Receiver Manager not fork S3");
	} else if (pid == 0) {
		char * argv[] = {
			NULL	
		};
		execvp("./S3", argv);
		ErrExit("S3 not start");
	}
	S3 = createChild(SENDER_3(), pid);
	printChild(SENDER_FILENAME, S3);
	
	// Wait the end of all child
	pid_t child;
	int status;
	while ((child = wait( & status)) != -1) {
		//printf("returned child %d with status %d\n", child, status);
	}

	printLog("SM", "Process End");
	return 0;
}