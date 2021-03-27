/// @file R1.c
/// @brief Contiene l'implementazione del receiver 1.

#include "../err_exit.h"
#include "../defines.h"
#include "../shared_memory.h"
#include "../semaphore.h"
#include "../fifo.h"
#include "../pipe.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char * argv[]) {

	printLog("R1", "Process start with exec");

    // ARGV: initSemId
    int initSemId = atoi(argv[0]);

    // Open SHM
    // Open MSGQ
    // OPEN PIPE R1 R2

    // Set this process as end init
    semOp(initSemId, 2, -1);

    // Wait all init end
    semOp(initSemId, 4, 0);

	time_t arrival;
	time_t departure;

	// Messaggio di test
	message *m = createMessage(
		1, 
		"Ciao come va?",
		SENDER_1(),
		RECEIVER_1(),
		1,
		1,
		1,
		"H"
	);
	time(&arrival);

	char log[50];
	sprintf(log, "Elaborated message: %d", m->id);
	//printLog("R1", log);
		
	time(&departure);

	trafficInfo *t = createTrafficInfo(m, arrival, departure);
	printTrafficInfo(RECEIVER_1_FILENAME, t);

	// Wait for 1 second befor end
	sleep(1);
	printLog("R1", "Process End");
	return 1;
}
